#pragma once

#include "CoreMinimal.h"
#include "EffectAssetBase.h"
#include "Sound/SoundBase.h"
#include "SoundEffectAsset.generated.h"

class UAudioComponent;

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API USoundEffectAsset : public UEffectAssetBase
{
    GENERATED_BODY()

public:
    USoundEffectAsset();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    USoundBase* SoundFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float LifeSpan;

    virtual UObject* StartEffect(AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation) override;

    virtual void StopEffect(UObject* EffectComponent) override;
};
