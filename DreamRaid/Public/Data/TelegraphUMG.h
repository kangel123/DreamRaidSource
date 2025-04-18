#pragma once

#include "CoreMinimal.h"
#include "TelegraphData.h"
#include "TelegraphUMG.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API UTelegraphUMG : public UTelegraphData
{
    GENERATED_BODY()

public:
    UTelegraphUMG();

    /** UMG 위젯 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Telegraph|UMG")
    TSubclassOf<class UUserWidget> UMGWidgetClass;

    virtual void StartTelegraph_Implementation(FVector Location, FRotator Rotation) override;
    virtual void StopTelegraph_Implementation() override;

protected:
    /** 생성된 위젯을 추적 */
    UPROPERTY(Transient)
    TWeakObjectPtr<class UUserWidget> UMGWidget;
};
