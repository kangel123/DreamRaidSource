#include "SkillRanged.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ProjectileInterface.h"   // 투사체 초기화를 위한 인터페이스
#include "StatInterface.h"         // IStatInterface를 통한 Attack 값 반환
#include "Components/SceneComponent.h"

USkillRanged::USkillRanged()
{
    SkillCategory = ESkillCategory::Ranged;
    ProjectileClass = nullptr;
    ProjectileSpeed = 2000.f;
    ProjectileLifetime = 5.0f;
    ProjectileSpawnOffset = FVector::ZeroVector;
    ProjectileSpawnComponent = nullptr;
    SpreadCount = 1;
    SpreadAngle = 0.f;
    LaunchDirectionType = ELaunchDirectionType::ActorForward;
    CustomLaunchDirection = FVector::ZeroVector;
}

float USkillRanged::CalculateFinalDamage(AActor* SkillOwner) const
{
    float AttackValue = 0.f;
    if (SkillOwner && SkillOwner->GetClass()->ImplementsInterface(UStatInterface::StaticClass()))
    {
        AttackValue = IStatInterface::Execute_GetAttack(SkillOwner);
    }
    return DamageRate * AttackValue;
}

FVector USkillRanged::CalculateLaunchDirection(AActor* SkillOwner, const FVector& SpawnLocation, const FVector& OptionalTargetLocation) const
{
    switch (LaunchDirectionType)
    {
    case ELaunchDirectionType::SpecificTarget:
        if (!OptionalTargetLocation.IsNearlyZero())
        {
            return (OptionalTargetLocation - SpawnLocation).GetSafeNormal();
        }
        [[fallthrough]];
    case ELaunchDirectionType::ActorForward:
        if (ProjectileSpawnComponent)
        {
            return ProjectileSpawnComponent->GetForwardVector();
        }
        return SkillOwner->GetActorForwardVector();
    case ELaunchDirectionType::Random:
        return FMath::VRand();
    case ELaunchDirectionType::Custom:
        return CustomLaunchDirection.GetSafeNormal();
    default:
        return SkillOwner->GetActorForwardVector();
    }
}

AActor* USkillRanged::SpawnProjectile(AActor* SkillOwner)
{
    if (!SkillOwner || !ProjectileClass)
    {
        return nullptr;
    }

    UWorld* World = SkillOwner->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    FVector BaseLocation = SkillOwner->GetActorLocation();
    if (ProjectileSpawnComponent)
    {
        BaseLocation = ProjectileSpawnComponent->GetComponentLocation();
    }
    FVector SpawnLocation = BaseLocation + ProjectileSpawnOffset;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = SkillOwner;
    AActor* Projectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SkillOwner->GetActorRotation(), SpawnParams);
    if (Projectile)
    {
        if (ProjectileSpawnComponent)
        {
            Projectile->AttachToComponent(ProjectileSpawnComponent, FAttachmentTransformRules::KeepWorldTransform);
        }
        else
        {
            Projectile->AttachToActor(SkillOwner, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
    return Projectile;
}

void USkillRanged::LaunchProjectile(AActor* SkillOwner, AActor* Projectile, const FVector& OptionalTargetLocation)
{
    if (!SkillOwner || !Projectile)
    {
        return;
    }

    Projectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    FVector CurrentLocation = Projectile->GetActorLocation();
    FVector LaunchDirection = CalculateLaunchDirection(SkillOwner, CurrentLocation, OptionalTargetLocation);
    float FinalDamage = CalculateFinalDamage(SkillOwner);

    if (Projectile->GetClass()->ImplementsInterface(UProjectileInterface::StaticClass()))
    {
        IProjectileInterface::Execute_InitializeProjectile(Projectile, FinalDamage, ProjectileSpeed, ProjectileLifetime, LaunchDirection);
    }
}

void USkillRanged::FireProjectile(AActor* SkillOwner, const FVector& OptionalTargetLocation)
{
    if (!SkillOwner || !ProjectileClass)
    {
        return;
    }

    UWorld* World = SkillOwner->GetWorld();
    if (!World)
    {
        return;
    }

    FVector BaseLocation = SkillOwner->GetActorLocation();
    if (ProjectileSpawnComponent)
    {
        BaseLocation = ProjectileSpawnComponent->GetComponentLocation();
    }
    FVector SpawnLocation = BaseLocation + ProjectileSpawnOffset;

    for (int32 i = 0; i < SpreadCount; i++)
    {
        float AngleOffset = (SpreadCount > 1) ? SpreadAngle * ((float)i - ((float)SpreadCount - 1) / 2.0f) : 0.f;
        FRotator SpawnRotation = SkillOwner->GetActorRotation();
        SpawnRotation.Yaw += AngleOffset;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = SkillOwner;
        AActor* Projectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (Projectile)
        {
            if (ProjectileSpawnComponent)
            {
                Projectile->AttachToComponent(ProjectileSpawnComponent, FAttachmentTransformRules::KeepWorldTransform);
            }
            else
            {
                Projectile->AttachToActor(SkillOwner, FAttachmentTransformRules::KeepWorldTransform);
            }
            LaunchProjectile(SkillOwner, Projectile, OptionalTargetLocation);
        }
    }
}
