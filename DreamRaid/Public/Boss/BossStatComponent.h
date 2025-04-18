#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossStatComponent.generated.h"

// 공격 방향 열거형은 앞서 정의된 것을 사용합니다.
UENUM(BlueprintType)
enum class EAttackDirection : uint8
{
    Front    UMETA(DisplayName = "Front"),
    Back     UMETA(DisplayName = "Back"),
    Other    UMETA(DisplayName = "Other")
};

// 체력 변화 델리게이트: 새 체력과 최대 체력을 전달
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UBossStatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBossStatComponent();

    virtual void BeginPlay() override;

    // 최대 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float MaxHealth = 1000000.f;

    // 현재 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
    float CurrentHealth = 1000000.f;

    // 최대 무력화 (스태거)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float MaxStagger = 100.f;

    // 현재 무력화
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
    float CurrentStagger = 100.f;

    // 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float MoveSpeed = 800.f;

    // 공격 비율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float AttackPowerMultiplier = 1.0f;

    // 방어력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Defense = 100.f;

    // 프론트 어택 당할 때의 공격 배율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float FrontAttackDamageMultiplier = 1.2f;

    // 백 어택 당할 때의 공격 배율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float BackAttackDamageMultiplier = 1.5f;

    // 기본 공격 배율 (Other)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float DefaultAttackDamageMultiplier = 1.0f;

    // 체력 퍼센트 반환 함수
    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetHPPercent() const;

    // 데미지 적용 함수 (공격 방향 고려)
    UFUNCTION(BlueprintCallable, Category="Stats")
    void ApplyDamage(float DamageAmount, EAttackDirection AttackDirection);

    // 체력 회복 함수
    UFUNCTION(BlueprintCallable, Category="Stats")
    void Heal(float HealAmount);

    // 무력화 데미지 적용 함수
    UFUNCTION(BlueprintCallable, Category="Stats")
    void ApplyStaggerDamage(float StaggerDamage);

    // 무력화 회복 함수 (최대 무력화로 복원)
    UFUNCTION(BlueprintCallable, Category="Stats")
    void RecoverStagger();

    // 체력 변화 델리게이트 (UI 갱신용)
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnHealthChanged OnHealthChanged;
};
