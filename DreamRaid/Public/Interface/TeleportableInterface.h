#pragma once

#include "UObject/Interface.h"
#include "TeleportableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTeleportableInterface : public UInterface
{
    GENERATED_BODY()
};

class ITeleportableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Teleport")
    void PrepareForTeleport();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Teleport")
    void FinishTeleport();
};
