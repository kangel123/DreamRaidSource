#include "PlayerSkillManagerComponent.h"
#include "GameFramework/Actor.h"
#include "CustomPlayerController.h"
#include "Engine/Engine.h"

UPlayerSkillManagerComponent::UPlayerSkillManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentSkill = nullptr;
}

void UPlayerSkillManagerComponent::ExecuteSkillByKey(const FKey& PressedKey)
{
    for (int32 i = 0; i < PlayerSkills.Num(); i++)
    {
        FPlayerSkillData& SkillData = PlayerSkills[i];
        if (SkillData.KeyBinding == PressedKey && SkillData.SkillInstance)
        {
            if (SkillData.SkillInstance->bIsOnCooldown)
            {
                continue;
            }

            AActor* Owner = GetOwner();
            if (!Owner)
            {
                continue;
            }
            
            CurrentSkill = SkillData.SkillInstance;
            CurrentSkill->ExecuteSkill(Owner, Owner->GetActorLocation());

            return;
        }
    }
}

void UPlayerSkillManagerComponent::ResetCurrentSkill()
{
    CurrentSkill = nullptr;
}


AActor* UPlayerSkillManagerComponent::SpawnSkillActor(TSubclassOf<AActor> ActorToSpawn, const FVector& Location, const FRotator& Rotation)
{
    if (!GetWorld() || !ActorToSpawn)
    {
        return nullptr;
    }
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = GetOwner()->GetInstigator();
    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, SpawnParams);
    if (SpawnedActor)
    {
        UE_LOG(LogTemp, Log, TEXT("SpawnSkillActor: Spawned actor at %s"), *Location.ToString());
    }
    return SpawnedActor;
}
