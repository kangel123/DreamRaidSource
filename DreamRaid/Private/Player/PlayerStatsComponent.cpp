#include "PlayerStatsComponent.h"
#include "Math/UnrealMathUtility.h"

UPlayerStatsComponent::UPlayerStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // 기본 스탯 값
    MaxHealth = 1000.f;
    MaxMana = 500.f;
    Attack = 150.f;
    Penetration = 50.f;
    CritChance = 0.2f;
    CritDamage = 1.5f;
    CooldownReduction = 0.f;
    SkillDamageBoost = 0.f;
    ManaRegen = 5.f;
    SkillCastingSpeed = 1.f;
    StatusResistance = 0.1f;
}

void UPlayerStatsComponent::InitializeStats()
{
    CurrentHealth = MaxHealth;
    CurrentMana = MaxMana;
}

void UPlayerStatsComponent::TakeDamage(float DamageAmount)
{
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
}

void UPlayerStatsComponent::Heal(float HealAmount)
{
    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
}

// 지정한 양만큼 마나를 증가시키는 함수 (예: 마나 포션 효과)
void UPlayerStatsComponent::RestoreMana(float ManaAmount)
{
    CurrentMana = FMath::Clamp(CurrentMana + ManaAmount, 0.f, MaxMana);
}

bool UPlayerStatsComponent::ConsumeMana(float ManaCost)
{
    if (CurrentMana >= ManaCost)
    {
        CurrentMana -= ManaCost;
        return true;
    }
    return false;
}

void UPlayerStatsComponent::RegenerateMana(float DeltaTime)
{
    CurrentMana = FMath::Clamp(CurrentMana + ManaRegen * DeltaTime, 0.f, MaxMana);
}

float UPlayerStatsComponent::GetAttack_Implementation() const
{
    return Attack;
}
