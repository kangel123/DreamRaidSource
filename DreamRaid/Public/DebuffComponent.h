#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeBuffInterface.h"
#include "PlayerStatsComponent.h" // 필요한 경우
#include "PlayerSkillManagerComponent.h" // 필요한 경우
#include "DebuffComponent.generated.h"

USTRUCT()
struct FDebuffData
{
    GENERATED_BODY()
    float RemainingTime = 0.f;
    float TickInterval  = 0.f;
    FTimerHandle TickHandle;
    FTimerHandle ExpireHandle;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UDebuffComponent : public UActorComponent, public IDeBuffInterface
{
    GENERATED_BODY()

public:
    UDebuffComponent();

    // IDeBuffInterface
    virtual void ApplyDebuff_Implementation(EDebuffType DebuffType, float Duration, float TickInterval) override;
    virtual void ResetDebuffTimer_Implementation(EDebuffType DebuffType, float NewDuration) override;
    virtual bool IsUnderDebuff_Implementation(EDebuffType DebuffType) const override;
    virtual void RemoveDebuff_Implementation(EDebuffType DebuffType) override;
    virtual float GetDebuffRemainingTime_Implementation(EDebuffType DebuffType) const override;
    virtual void UpdateDebuffEffect_Implementation(EDebuffType DebuffType) override;
    virtual void OnDebuffExpired_Implementation(EDebuffType DebuffType) override;

private:
    /** 각 DebuffType 별로 타이머와 남은시간 관리 */
    UPROPERTY()
    TMap<EDebuffType, FDebuffData> ActiveDebuffs;

    /** 헬퍼들 */
    void ScheduleExpire(EDebuffType DebuffType, float Duration);
    void ScheduleTick(EDebuffType DebuffType, float Interval);

    /** Darkness 전용 변수 (예시) */
    UPROPERTY(EditAnywhere, Category="Debuff|Darkness")
    UMaterialInstance* DarknessMaterialTemplate;
    UPROPERTY()
    UMaterialInstanceDynamic* DarknessMID;
    bool bDarknessActive       = false;
    float CurrentDarknessFade  = 0.f;
    float DarknessFadeSpeed    = 1.f;
};
