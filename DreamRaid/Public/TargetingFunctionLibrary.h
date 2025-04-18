#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TargetingFunctionLibrary.generated.h"

UCLASS()
class DREAMRAID_API UTargetingFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Targeting")
    static AActor* GetActorUnderCursor(APlayerController* PlayerController, bool bDrawDebug = false);

    UFUNCTION(BlueprintCallable, Category = "Targeting")
    static bool IsValidSkillTarget(AActor* TargetActor);
};
