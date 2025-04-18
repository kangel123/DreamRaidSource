#pragma once

#include "CoreMinimal.h"
#include "EffectAssetBase.h"
#include "DecalEffectAsset.generated.h"

class UDecalComponent;
class UMaterialInterface;

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UDecalEffectAsset : public UEffectAssetBase
{
    GENERATED_BODY()

public:
    UDecalEffectAsset();

    // 데칼에 사용할 재질
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal Effect")
    UMaterialInterface* DecalMaterial;

    // 데칼 크기 (예: FVector(200,200,200))
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal Effect")
    FVector DecalSize;

    // 데칼의 수명 (초). 0이면 영구적으로 유지됨.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal Effect")
    float LifeSpan;

    virtual UObject* StartEffect(AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation) override;
    virtual void StopEffect(UObject* EffectComponent) override;
};
