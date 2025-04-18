#include "BossEnrageComponent.h"
#include "Boss.h"  // ABoss 헤더 포함
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraShakeBase.h"

UBossEnrageComponent::UBossEnrageComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBossEnrageComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 설정된 EnrageTime 후에 Enrage 모드 시작
    if(GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(EnrageTimerHandle, this, &UBossEnrageComponent::StartEnrageMode, EnrageTime, false);
    }
}

void UBossEnrageComponent::StartEnrageMode()
{
    // 광폭화 효과 재생
    if (EnrageSoundCue)
    {
        // 아직 안 넣었으니 패스함
    }
    if (EnrageNiagaraSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnrageNiagaraSystem, GetOwner()->GetActorLocation());
    }
    if (EnrageCameraShake)
    {
        UGameplayStatics::PlayWorldCameraShake(GetWorld(), EnrageCameraShake, GetOwner()->GetActorLocation(), 0.f, 1000.f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Boss has entered Enrage mode."));
    
    // 주기적으로 회복 처리 시작
    if(GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(EnrageRegenTimerHandle, this, &UBossEnrageComponent::EnrageRegenTick, EnrageRegenInterval, true);
    }
}

void UBossEnrageComponent::EnrageRegenTick()
{
    // 보스(ABoss)로 캐스팅해서 체력 회복 함수를 호출합니다.
    if (AActor* OwnerActor = GetOwner())
    {
        if (ABoss* BossOwner = Cast<ABoss>(OwnerActor))
        {
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Enrage Regen Tick: Regenerating %f health."), EnrageRegenAmount);
}

void UBossEnrageComponent::StopEnrageMode()
{
    if(GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(EnrageRegenTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(EnrageTimerHandle);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Boss has exited Enrage mode."));
}
