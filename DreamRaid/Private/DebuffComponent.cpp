#include "DebuffComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UDebuffComponent::UDebuffComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDebuffComponent::ApplyDebuff_Implementation(EDebuffType Type, float Duration, float TickInterval)
{
    FDebuffData& D = ActiveDebuffs.FindOrAdd(Type);
    D.RemainingTime = Duration;
    D.TickInterval  = TickInterval;

    // 이미 활성 중이면 만료 타이머만 연장
    if (GetWorld()->GetTimerManager().IsTimerActive(D.ExpireHandle))
    {
        ScheduleExpire(Type, Duration);
        return;
    }

    // Tick & Expire 타이머 설정
    ScheduleTick(Type, TickInterval);
    ScheduleExpire(Type, Duration);

    // Darkness 특수 처리
    if (Type == EDebuffType::Darkness)
    {
        bDarknessActive      = true;
        CurrentDarknessFade  = 0.f;
        if (DarknessMaterialTemplate)
        {
            DarknessMID = DarknessMID
              ? DarknessMID
              : UMaterialInstanceDynamic::Create(DarknessMaterialTemplate, GetOwner());
            if (auto Cam = GetOwner()->FindComponentByClass<UCameraComponent>())
                Cam->PostProcessSettings.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, DarknessMID));
            DarknessMID->SetScalarParameterValue(TEXT("FadeStrength"), 0.f);
        }
    }
}

void UDebuffComponent::ResetDebuffTimer_Implementation(EDebuffType Type, float NewDuration)
{
    if (auto* D = ActiveDebuffs.Find(Type))
    {
        D->RemainingTime = NewDuration;
        ScheduleExpire(Type, NewDuration);
    }
}

bool UDebuffComponent::IsUnderDebuff_Implementation(EDebuffType Type) const
{
    if (auto* D = ActiveDebuffs.Find(Type))
        return GetWorld()->GetTimerManager().IsTimerActive(D->ExpireHandle);
    return false;
}

void UDebuffComponent::RemoveDebuff_Implementation(EDebuffType Type)
{
    if (auto* D = ActiveDebuffs.Find(Type))
    {
        // 타이머 클리어
        auto& TM = GetWorld()->GetTimerManager();
        TM.ClearTimer(D->TickHandle);
        TM.ClearTimer(D->ExpireHandle);

        // 특수 처리
        if (Type == EDebuffType::Darkness && DarknessMID)
        {
            bDarknessActive     = false;
            CurrentDarknessFade = 0.f;
            DarknessMID->SetScalarParameterValue(TEXT("FadeStrength"), 0.f);
        }

        OnDebuffExpired(Type);
        ActiveDebuffs.Remove(Type);
    }
}

float UDebuffComponent::GetDebuffRemainingTime_Implementation(EDebuffType Type) const
{
    if (auto* D = ActiveDebuffs.Find(Type))
        return D->RemainingTime;
    return 0.f;
}

void UDebuffComponent::UpdateDebuffEffect_Implementation(EDebuffType Type)
{
    if (auto* D = ActiveDebuffs.Find(Type))
    {
        D->RemainingTime = FMath::Max(D->RemainingTime - D->TickInterval, 0.f);

        // 디버프별 효과
        if (Type == EDebuffType::Darkness && bDarknessActive && DarknessMID)
        {
            CurrentDarknessFade = FMath::Clamp(CurrentDarknessFade + D->TickInterval * DarknessFadeSpeed, 0.f, 1.f);
            DarknessMID->SetScalarParameterValue(TEXT("FadeStrength"), CurrentDarknessFade);
        }
        // else if (Type == EDebuffType::Burn) { /* DOT 처리 */ }
    }
}

void UDebuffComponent::OnDebuffExpired_Implementation(EDebuffType Type)
{
    // 만료 후 처리 (UI 등)
}

void UDebuffComponent::ScheduleTick(EDebuffType Type, float Interval)
{
    FDebuffData& D = ActiveDebuffs[Type];
    GetWorld()->GetTimerManager().SetTimer(
        D.TickHandle,
        FTimerDelegate::CreateUObject(this, &UDebuffComponent::UpdateDebuffEffect, Type),
        Interval, true
    );
}

void UDebuffComponent::ScheduleExpire(EDebuffType Type, float Duration)
{
    FDebuffData& D = ActiveDebuffs[Type];
    GetWorld()->GetTimerManager().SetTimer(
        D.ExpireHandle,
        FTimerDelegate::CreateUObject(this, &UDebuffComponent::RemoveDebuff, Type),
        Duration, false
    );
}
