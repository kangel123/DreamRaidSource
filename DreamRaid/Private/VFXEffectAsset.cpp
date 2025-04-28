// VFXEffectAsset.cpp
#include "VFXEffectAsset.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"

UVFXEffectAsset::UVFXEffectAsset()
{
    NiagaraSystem = nullptr;
}

UObject* UVFXEffectAsset::StartEffect_Implementation(AActor* Owner, FVector Location, FRotator Rotation)
{
    if (!NiagaraSystem || !Owner)
       return nullptr;

    const bool bUseSpawnTransform =
       Attachment.AttachType == EEffectAttachType::None ||
       Attachment.AttachType == EEffectAttachType::ToActor;

   // 최종 위치/회전 계산
    const FVector SpawnLocation = bUseSpawnTransform
        ? (Location + VFXSettings.LocationOffset)
        : (Owner->GetActorLocation() + VFXSettings.LocationOffset);

    const FRotator SpawnRotation = bUseSpawnTransform
        ? (Rotation + VFXSettings.RotationOffset)
        : (Owner->GetActorRotation() + VFXSettings.RotationOffset);

    UNiagaraComponent* Comp = nullptr;
    switch (Attachment.AttachType)
    {
        case EEffectAttachType::ToComponent:
            Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(
                NiagaraSystem,
                Attachment.ParentComponent,
                NAME_None,
                VFXSettings.LocationOffset,
                VFXSettings.RotationOffset,
                EAttachLocation::KeepRelativeOffset,
                !Lifetime.bLooping);
            break;

        case EEffectAttachType::ToSocket:
            Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(
                NiagaraSystem,
                Attachment.ParentComponent,
                Attachment.AttachSocketName,
                FVector::ZeroVector,
                FRotator::ZeroRotator,
                EAttachLocation::SnapToTarget,
                !Lifetime.bLooping);
            break;

        case EEffectAttachType::ToActor:
        case EEffectAttachType::None:
        default:
            Comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    Owner->GetWorld(), NiagaraSystem, SpawnLocation, SpawnRotation, VFXSettings.Scale, !Lifetime.bLooping, true);
            break;
    }

    // 루프 모드라면 MaxDuration 이후에 자동 정지
    if (Comp && Lifetime.bLooping && Lifetime.MaxDuration > 0.0f)
    {
        FTimerDelegate Del;
        Del.BindUFunction(this, FName("StopEffect"), Comp);
        Owner->GetWorldTimerManager().SetTimer(
            *new FTimerHandle(), Del, Lifetime.MaxDuration, false);
    }

    return Comp;
}

void UVFXEffectAsset::StopEffect_Implementation(UObject* EffectHandle)
{
    if (UNiagaraComponent* Comp = Cast<UNiagaraComponent>(EffectHandle))
    {
        Comp->Deactivate();
        if (Lifetime.bLooping)
        {
            Comp->DestroyComponent();
        }
    }
}
