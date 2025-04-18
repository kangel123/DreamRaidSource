#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossPhaseData.h"
#include "BossSkillData.h"
#include "BossSkillManager.generated.h"

/*
 * 스킬 구조체와 페이즈 구조체를 각각 테이블로 저장하고 사용할 스킬을 선택을 도와주는 클래스
 */

// 델리게이트 : 환경 효과
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnvironmentalEffectsTriggered, const TArray<FName>&, EnvironmentalEffectTriggers);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UBossSkillManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UBossSkillManager();

    virtual void BeginPlay() override;

public:
    // 스킬 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
    UDataTable* SkillDataTable;

    // 페이즈 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
    UDataTable* PhaseDataTable;

    // 현재 페이즈 인덱스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Skills")
    int32 CurrentPhaseIndex;

    // 환경 효과 델리게이트 (페이즈 전환 시 방송)
    UPROPERTY(BlueprintAssignable, Category="Effects")
    FOnEnvironmentalEffectsTriggered OnEnvironmentalEffectsTriggered;

protected:
    // 스킬 데이터 테이블
    UPROPERTY()
    TArray<FBossSkillData> AllSkills;
    
    // 페이즈 데이터 테이블
    UPROPERTY()
    TArray<FBossPhaseData> AllPhases;
    
public:
    // 스킬 선택 함수
    UFUNCTION(BlueprintCallable, Category="Skills")
    FBossSkillData ChooseSkill(float CurrentHealthPercent, float DistanceToPlayer);

    // 현재 시간을 반환 (쿨다운 계산용)
    double GetCurrentTime() const { return FPlatformTime::Seconds(); }
protected:
    
    void LoadSkillData();
    void LoadPhaseData();
    
    void UpdatePhase(float CurrentHealthPercent);
};
