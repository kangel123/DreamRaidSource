#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatInterface.h"
#include "PlayerStatsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UPlayerStatsComponent : public UActorComponent, public IStatInterface
{
    GENERATED_BODY()

public:
    UPlayerStatsComponent();

    // 체력 관련 스탯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float MaxHealth;          // 최대 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
    float CurrentHealth;      // 현재 체력

    // 마나 관련 스탯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float MaxMana;            // 최대 마나
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
    float CurrentMana;        // 현재 마나

    // 전투 및 스킬 관련 스탯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Attack;        // 공격력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Penetration;   // 관통력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float CritChance;         // 치명타 확률
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float CritDamage;         // 치명타 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float CooldownReduction;  // 스킬 쿨다운 감소
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float SkillDamageBoost;   // 스킬 데미지 증가
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float ManaRegen;          // 마나 재생 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float SkillCastingSpeed;  // 스킬 캐스팅 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float StatusResistance;   // 상태 이상 저항력

    // 함수들
    UFUNCTION(BlueprintCallable, Category="Stats")
    void InitializeStats(); // 초기화
    UFUNCTION(BlueprintCallable, Category="Stats")
    void TakeDamage(float DamageAmount);    // 체력 감소
    UFUNCTION(BlueprintCallable, Category="Stats")
    void Heal(float HealAmount);    // 체력회복
    UFUNCTION(BlueprintCallable, Category="Stats")
    void RestoreMana(float ManaAmount); // 마나 회복
    UFUNCTION(BlueprintCallable, Category="Stats")
    bool ConsumeMana(float ManaCost);   // 마나 감소
    UFUNCTION(BlueprintCallable, Category="Stats")
    void RegenerateMana(float DeltaTime);   // 자연 마나 회복
    
    virtual float GetAttack_Implementation() const override;
};
