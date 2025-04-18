#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatInterface.generated.h"

UINTERFACE(BlueprintType)
class DREAMRAID_API UStatInterface : public UInterface
{
    GENERATED_BODY()
};

class DREAMRAID_API IStatInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats")
    float GetAttack() const;
    virtual float GetAttack_Implementation() const { return 0.f; }
};
