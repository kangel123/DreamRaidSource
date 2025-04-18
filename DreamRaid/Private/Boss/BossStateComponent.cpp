#include "BossStateComponent.h"

UBossStateComponent::UBossStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    bCanMove = true;
    bCanAttack = true;
    bIsEnraged = false;
    bIsStaggerImmune = false;
    bIsInvincible = false;

    CurrentActionState = EBossActionState::Idle;
}

void UBossStateComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UBossStateComponent::SetEnraged(bool bNewEnraged)
{
    bIsEnraged = bNewEnraged;
}

void UBossStateComponent::SetStaggerImmunity(bool bNewStaggerImmune)
{
    if (bIsStaggerImmune != bNewStaggerImmune)
    {
        bIsStaggerImmune = bNewStaggerImmune;
        OnStaggerImmunityChanged.Broadcast(bIsStaggerImmune);
    }
}

void UBossStateComponent::SetInvincible(bool bNewInvincible)
{
    bIsInvincible = bNewInvincible;
}

void UBossStateComponent::SetActionState(EBossActionState NewState)
{
    if (CurrentActionState != NewState)
    {
        CurrentActionState = NewState;
        OnActionStateChanged.Broadcast(CurrentActionState);
    }
}
