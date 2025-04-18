#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectAssetBase.generated.h"

UCLASS(Abstract, EditInlineNew, BlueprintType, DefaultToInstanced)
class DREAMRAID_API UEffectAssetBase : public UObject
{
    GENERATED_BODY()

public:
    UEffectAssetBase();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool bReuseExistingComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (EditCondition = "bReuseExistingComponent"))
    USceneComponent* PreExistingComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName ComponentAttachSocket;


    UFUNCTION(BlueprintCallable, Category = "Effect")
    virtual UObject* StartEffect(AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation)
        PURE_VIRTUAL(UEffectAssetBase::StartEffect, return nullptr;);

    UFUNCTION(BlueprintCallable, Category = "Effect")
    virtual void StopEffect(UObject* EffectComponent)
        PURE_VIRTUAL(UEffectAssetBase::StopEffect, );
};
