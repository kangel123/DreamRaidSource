#pragma once

#include "CoreMinimal.h"
#include "EffectAssetBase.h"
#include "NiagaraSystem.h"
#include "VisualEffectAsset.generated.h"

class UNiagaraComponent;

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UVisualEffectAsset : public UEffectAssetBase
{
    GENERATED_BODY()

public:
    UVisualEffectAsset();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UNiagaraSystem* VisualFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float LifeSpan;

    virtual UObject* StartEffect(AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation) override;

    virtual void StopEffect(UObject* EffectComponent) override;
};
