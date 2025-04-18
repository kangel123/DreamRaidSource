#pragma once

#include "CoreMinimal.h"
#include "TelegraphData.h"
#include "TelegraphCustom.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UTelegraphCustom : public UTelegraphData
{
    GENERATED_BODY()

public:
    UTelegraphCustom();

    virtual void StartTelegraph_Implementation(FVector Location, FRotator Rotation) override;
    virtual void StopTelegraph_Implementation() override;
};
