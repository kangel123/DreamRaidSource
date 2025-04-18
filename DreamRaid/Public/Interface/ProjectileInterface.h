#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"

UINTERFACE(BlueprintType)
class DREAMRAID_API UProjectileInterface : public UInterface
{
    GENERATED_BODY()
};

class DREAMRAID_API IProjectileInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
    void InitializeProjectile(float FinalDamage, float ProjectileSpeed, float ProjectileLifetime, FVector LaunchDirection);
};
