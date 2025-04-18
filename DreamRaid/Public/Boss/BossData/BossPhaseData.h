#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BossSkillData.h"
#include "BossPhaseData.generated.h"

UENUM(BlueprintType)
enum class EPhaseTriggerCondition : uint8
{
    HPBased      UMETA(DisplayName = "HP Based"),
    TimeBased    UMETA(DisplayName = "Time Based"),
    Custom       UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FBossPhaseData : public FTableRowBase
{
    GENERATED_BODY()

    // 페이즈 번호
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    int32 PhaseIndex;

    // 페이즈 변경 조건
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    EPhaseTriggerCondition TriggerCondition = EPhaseTriggerCondition::HPBased;
    
    // 체력 조건
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    float TriggerHPPercent = 1.0f;

    // 시간 조건
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase", meta=(EditCondition="TriggerCondition == EPhaseTriggerCondition::TimeBased"))
    float PhaseDuration = 0.f;

    // 일회용 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    bool bTriggerOnlyOnce = true;

    // 페이즈 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    FName PhaseName;

    // 사용할 스킬들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    TArray<FBossSkillData> PhaseSkills;

    // 페이즈 변경 시 사용할 몽타주
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    FString PhaseTransitionMontage;

    // 환경 효과 트리거들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Phase")
    TArray<FName> EnvironmentalEffectTriggers;
};
