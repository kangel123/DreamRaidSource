#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class DREAMRAID_API ACustomPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACustomPlayerController();
    virtual void Tick(float DeltaSeconds) override;
    
protected:
    virtual void SetupInputComponent() override;

    // 이동 관련 입력 (우클릭)
    UFUNCTION()
    void OnRightClickPressed();
    UFUNCTION()
    void OnRightClickReleased();
    void ProcessMoveToDestination();
    
    // 스킬 입력: 기본 공격, 회피, 일반 스킬 (키에 따른 처리)
    UFUNCTION()
    void OnBasicAttackPressed();
    UFUNCTION()
    void OnDodgePressed();

    // 예시: 일반 스킬 Q, W, E, R, A, S, D, F
    UFUNCTION()
    void OnNormalSkillQPressed();
    UFUNCTION()
    void OnNormalSkillWPressed();
    UFUNCTION()
    void OnNormalSkillEPressed();
    UFUNCTION()
    void OnNormalSkillRPressed();
    UFUNCTION()
    void OnNormalSkillAPressed();
    UFUNCTION()
    void OnNormalSkillSPressed();
    UFUNCTION()
    void OnNormalSkillDPressed();
    UFUNCTION()
    void OnNormalSkillFPressed();
    
public:
    void ClearMoveRequest() { bHasValidTarget = false; }
private:
    // 우클릭으로 지정한 목표 위치
    FVector TargetLocation;
    bool bHasValidTarget;
    bool bIsRightClickHeld;
};
