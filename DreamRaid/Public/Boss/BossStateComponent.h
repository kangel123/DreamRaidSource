#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossActionState.h"
#include "BossStateComponent.generated.h"

/**
 * UBossStateComponent
 * 보스의 특수 상태(광폭화, 무력화 면역, 무적)와 현재 행동 상태를 관리하는 컴포넌트.
 */
 
// 행동 상태 변경 델리게이트 (외부에 상태 변화 이벤트 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionStateChanged, TEnumAsByte<EBossActionState>, NewState);

// 무력화 면역 상태 변경 델리게이트 (UI 업데이트 등 외부 처리용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaggerImmunityChanged, bool, bNewStaggerImmune);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UBossStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBossStateComponent();

    virtual void BeginPlay() override;

    // 이동 가능 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Boss State")
    bool bCanMove;

    // 공격 가능 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Boss State")
    bool bCanAttack;
    
    // 광폭화 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
    bool bIsEnraged;

    // 무력화 면역 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
    bool bIsStaggerImmune;

    // 무적 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
    bool bIsInvincible;

    // 현재 행동 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
    TEnumAsByte<EBossActionState> CurrentActionState;

    // 델리게이트: 행동 상태 변경 시 방송
    UPROPERTY(BlueprintAssignable, Category="State")
    FOnActionStateChanged OnActionStateChanged;

    // 델리게이트: 무력화 면역 상태 변경 시 방송
    UPROPERTY(BlueprintAssignable, Category="State")
    FOnStaggerImmunityChanged OnStaggerImmunityChanged;

    // 광폭화 상태 변경 함수
    UFUNCTION(BlueprintCallable, Category="State")
    void SetEnraged(bool bNewEnraged);

    // 무력화 면역 상태 변경 함수
    UFUNCTION(BlueprintCallable, Category="State")
    void SetStaggerImmunity(bool bNewStaggerImmune);

    // 무적 상태 변경 함수
    UFUNCTION(BlueprintCallable, Category="State")
    void SetInvincible(bool bNewInvincible);

    // 행동 상태 변경 함수
    UFUNCTION(BlueprintCallable, Category="State")
    void SetActionState(EBossActionState NewState);
};
