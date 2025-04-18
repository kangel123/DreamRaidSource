#pragma once

#include "CoreMinimal.h"
#include "BossActionState.generated.h"

UENUM(BlueprintType)
enum EBossActionState : uint8
{
    Idle         UMETA(DisplayName = "Idle"),
    Moving       UMETA(DisplayName = "Moving"),
    Dodge      UMETA(DisplayName = "Dodge"),
    Attacking    UMETA(DisplayName = "Attacking"),
    Groggy      UMETA(DisplayName = "Groggy"),
    Dead         UMETA(DisplayName = "Dead")
};
