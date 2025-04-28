#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DeBuffInterface.h"
#include "PlayerStatsComponent.h"
#include "PlayerSkillManagerComponent.h"
#include "SkillUser.h"
#include "TeleportableInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LostArkCharacter.generated.h"

UENUM(BlueprintType)
enum class ELostArkCharacterState : uint8
{
    Normal      UMETA(DisplayName = "Normal"),
    Down        UMETA(DisplayName = "Down"),
    Attacking   UMETA(DisplayName = "Attacking"),
    Airborne    UMETA(DisplayName = "Airborne"),
    Stunned     UMETA(DisplayName = "Stunned"),
    Dodging     UMETA(DisplayName = "Dodging")
};

UCLASS()
class DREAMRAID_API ALostArkCharacter : public ACharacter, public IDeBuffInterface, public ISkillUser, public ITeleportableInterface
{
    GENERATED_BODY()

public:
    ALostArkCharacter();

protected:
    virtual void BeginPlay() override;

public:
    // 스탯 및 스킬 관리 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPlayerStatsComponent* PlayerStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPlayerSkillManagerComponent* SkillManager;

    // TopDown 카메라 구성용 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* TopDownCameraComponent;

    // 플레이어 현재 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    ELostArkCharacterState CurrentState;
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Skill")
    USkillData* GetActiveSkillData() const;
    
    // 현재 활성 스킬 데이터 (PlayerSkillManagerComponent에서 관리할 수도 있으나, 이 예에서는 캐릭터에서 반환)
    UPROPERTY(BlueprintReadOnly, Category="Skill")
    class USkillData* CurrentSkillData;

    // 목표 좌표로 이동하는 함수
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToDestination(const FVector& Destination);

    // 상태 변경 함수
    UFUNCTION(BlueprintCallable, Category = "State")
    void SetCharacterState(ELostArkCharacterState NewState);

    // ========== IDeBuffInterface 구현 ==========
    virtual void ApplyDebuff_Implementation(EDebuffType DebuffType, float Duration, float TickInterval) override;
    virtual void ResetDebuffTimer_Implementation(EDebuffType DebuffType, float NewDuration) override;
    virtual bool IsUnderDebuff_Implementation(EDebuffType DebuffType) const override;
    virtual void RemoveDebuff_Implementation(EDebuffType DebuffType) override;
    virtual float GetDebuffRemainingTime_Implementation(EDebuffType DebuffType) const override;
    virtual void UpdateDebuffEffect_Implementation(EDebuffType DebuffType) override;
    virtual void OnDebuffExpired_Implementation(EDebuffType DebuffType) override;
    
    // ========== ISkillUser 구현 ==========
    virtual USkillData* GetActiveSkillData_Implementation() const override;
    
    // ========== ITeleportableInterface 구현 ==========
    virtual void PrepareForTeleport_Implementation() override;
    virtual void FinishTeleport_Implementation() override;
    
    bool bCanMove = true;
};
