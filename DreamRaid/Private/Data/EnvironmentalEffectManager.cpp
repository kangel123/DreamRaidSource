#include "EnvironmentalEffectManager.h"
#include "Kismet/GameplayStatics.h"

UEnvironmentalEffectManager::UEnvironmentalEffectManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEnvironmentalEffectManager::BeginPlay()
{
    Super::BeginPlay();
}

void UEnvironmentalEffectManager::TriggerEnvironmentalEffects(const TArray<FName>& EnvironmentalEffectTriggers)
{
    for (const FName& EffectName : EnvironmentalEffectTriggers)
    {
        UE_LOG(LogTemp, Log, TEXT("Triggering environmental effect: %s"), *EffectName.ToString());
        // 구현은 아직 안함
        // 나중에 트리거 이름에 따라 각기 다른 환경 효과 트리거를 실행할 예정
        // 즉, 여기서는 스위치 작업하면 될 듯
        // 나중에 새로운 클래스로 환경 효과들 만들면 편해 보임.
    }
}
