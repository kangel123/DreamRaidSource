#pragma once

#include "CoreMinimal.h"
#include "DebuffTypes.generated.h"

UENUM(BlueprintType)
enum class EDebuffType : uint8
{
    None        UMETA(DisplayName = "None"),
    Burn        UMETA(DisplayName = "Burn"),
    Freeze      UMETA(DisplayName = "Freeze"),
    Darkness    UMETA(DisplayName = "Darkness")
};
