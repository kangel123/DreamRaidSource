#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectSettings.h"
#include "EffectAssetBase.generated.h"

UCLASS(Abstract, EditInlineNew, BlueprintType)
class DREAMRAID_API UEffectAssetBase : public UObject
{
    GENERATED_BODY()

public:
    UEffectAssetBase();

    /** 부착 설정 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    FEffectAttachmentSettings Attachment;

    /** 수명 설정 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    FEffectLifetimeSettings Lifetime;

    /** 효과 시작 – 리턴값은 생성된 컴포넌트나 핸들로 쓰입니다 */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Effect")
    UObject* StartEffect(AActor* Owner, FVector Location, FRotator Rotation);
    virtual UObject* StartEffect_Implementation(AActor* Owner, FVector Location, FRotator Rotation) PURE_VIRTUAL(UEffectAssetBase::StartEffect_Implementation, return nullptr;);

    /** 효과 중지 */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Effect")
    void StopEffect(UObject* EffectHandle);
    virtual void StopEffect_Implementation(UObject* EffectHandle) PURE_VIRTUAL(UEffectAssetBase::StopEffect_Implementation, );
};
