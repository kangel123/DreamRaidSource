// SFXEffectAsset.h
#pragma once

#include "CoreMinimal.h"
#include "EffectAssetBase.h"
#include "Sound/SoundBase.h"
#include "SFXEffectAsset.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API USFXEffectAsset : public UEffectAssetBase
{
    GENERATED_BODY()

public:
    USFXEffectAsset();

    /** 재생할 사운드 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
    USoundBase* Sound;

    /** Sound 전용 볼륨/피치 세팅 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
    FEffectSoundSettings SoundSettings;

    // 현재 코드는 None 또는 ToActor일때만 Location과 Rotation를 통해 영향을 끼칠 수가 있도록 되어있다.
    // 나중에 변경될 가능성이 있다.
    virtual UObject* StartEffect_Implementation(AActor* Owner, FVector Location, FRotator Rotation) override;
    virtual void StopEffect_Implementation(UObject* EffectHandle) override;
};
