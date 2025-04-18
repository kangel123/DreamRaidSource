#pragma once

#include "CoreMinimal.h"
#include "TelegraphData.h"
#include "TelegraphDecal.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UTelegraphDecal : public UTelegraphData
{
    GENERATED_BODY()

public:
    UTelegraphDecal();

    /** Decal 재료 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telegraph|Decal")
    UMaterialInterface* DecalMaterial;

    virtual void StartTelegraph_Implementation(FVector Location, FRotator Rotation) override;
    virtual void StopTelegraph_Implementation() override;

protected:
    /** 스폰된 Decal 컴포넌트를 추적 */
    UPROPERTY(Transient)
    TWeakObjectPtr<class UDecalComponent> DecalComponent;
};
