// VFXEffectAsset.h
#pragma once

#include "CoreMinimal.h"
#include "EffectAssetBase.h"
#include "NiagaraSystem.h"
#include "VFXEffectAsset.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UVFXEffectAsset : public UEffectAssetBase
{
    GENERATED_BODY()

public:
    UVFXEffectAsset();

    /** 사용할 NiagaraSystem */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    UNiagaraSystem* NiagaraSystem;

    /** VFX 전용 Transform 세팅 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    FEffectVFXSettings VFXSettings;

    virtual UObject* StartEffect_Implementation(AActor* Owner, FVector Location, FRotator Rotation) override;
    virtual void StopEffect_Implementation(UObject* EffectHandle) override;
};
