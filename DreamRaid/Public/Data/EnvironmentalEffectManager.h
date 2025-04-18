#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentalEffectManager.generated.h"

/**
 * UEnvironmentalEffectManager 클래스
 * - BossSkillManager로부터 전달받은 환경 효과 트리거들을 처리하여,
 *   게임 내 환경 효과(예: 화면 효과, 음향 효과 등)를 실행합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UEnvironmentalEffectManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UEnvironmentalEffectManager();

    virtual void BeginPlay() override;

    // 환경 효과 트리거 배열을 받아 실행하는 함수
    UFUNCTION(BlueprintCallable, Category="Effects")
    void TriggerEnvironmentalEffects(const TArray<FName>& EnvironmentalEffectTriggers);
};
