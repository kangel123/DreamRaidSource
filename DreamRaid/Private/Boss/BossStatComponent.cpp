#include "BossStatComponent.h"
// #include "BossSkillManager.h"
#include "BossStateComponent.h"
#include "Math/UnrealMathUtility.h"

UBossStatComponent::UBossStatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBossStatComponent::BeginPlay()
{
    Super::BeginPlay();
}

float UBossStatComponent::GetHPPercent() const
{
    return (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
}

void UBossStatComponent::ApplyDamage(float DamageAmount, EAttackDirection AttackDirection)
{
    if (DamageAmount <= 0.f)
    {
        return;
    }
    
    // 공격 방향에 따른 배율 적용
    float AttackMultiplier = DefaultAttackDamageMultiplier;
    if (AttackDirection == EAttackDirection::Front)
    {
        AttackMultiplier = FrontAttackDamageMultiplier;
    }
    else if (AttackDirection == EAttackDirection::Back)
    {
        AttackMultiplier = BackAttackDamageMultiplier;
    }
    
    // 방어력 적용
    float EffectiveDamage = DamageAmount * AttackMultiplier * (100.f / (100.f + Defense));
    CurrentHealth = FMath::Max(CurrentHealth - EffectiveDamage, 0.f);
    
    // 체력 변화 이벤트 방송 (UI에서 사용)
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
    
    // 체력에 따른 페이즈 변화
/*    
    if (AActor* OwnerActor = GetOwner())
    {
        UBossSkillManager* SkillManager = OwnerActor->FindComponentByClass<UBossSkillManager>();
        if (SkillManager)
        {
            SkillManager->UpdatePhase(GetHPPercent());
        }
    }
 */
}

void UBossStatComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.f)
    {
        return;
    }
    
    CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UBossStatComponent::ApplyStaggerDamage(float StaggerDamage)
{
    if (StaggerDamage <= 0.f)
    {
        return;
    }
    
    // 무력화 수치를 감소시키고 최소 0으로 제한
    CurrentStagger = FMath::Max(CurrentStagger - StaggerDamage, 0.f);

    // 무력화 수치가 0이면, 상태 컴포넌트를 통해 무력화 면역 활성화 및 행동 상태 변경
    if (CurrentStagger <= 0.f)
    {
        if (AActor* OwnerActor = GetOwner())
        {
            // 보스 상태 컴포넌트를 찾습니다.
            UBossStateComponent* StateComp = OwnerActor->FindComponentByClass<UBossStateComponent>();
            if (StateComp)
            {
                // 무력화 면역 상태 활성화
                StateComp->SetStaggerImmunity(true);
                StateComp->SetActionState(EBossActionState::Groggy);
            }
        }
    }
}

void UBossStatComponent::RecoverStagger()
{
    CurrentStagger = MaxStagger;
    
    if (AActor* OwnerActor = GetOwner())
    {
        UBossStateComponent* StateComp = OwnerActor->FindComponentByClass<UBossStateComponent>();
        if (StateComp)
        {
            StateComp->SetStaggerImmunity(false);
        }
    }
}
