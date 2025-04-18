#pragma once

#include "CoreMinimal.h"
#include "TelegraphData.h"
#include "TelegraphNiagara.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UTelegraphNiagara : public UTelegraphData
{
    GENERATED_BODY()

public:
    UTelegraphNiagara();

    /** Niagara 효과 에셋 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telegraph|Niagara")
    UNiagaraSystem* NiagaraEffect;

    virtual void StartTelegraph_Implementation(FVector Location, FRotator Rotation) override;
    virtual void StopTelegraph_Implementation() override;

protected:
    /** 스폰된 Niagara 컴포넌트를 추적 */
    UPROPERTY(Transient)
    TWeakObjectPtr<class UNiagaraComponent> NiagaraComponent;
};
