#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraSystem.h"
#include "Materials/MaterialInterface.h"
#include "Blueprint/UserWidget.h"
// 반드시 다른 include 문 이후에 generated header를 포함합니다.
#include "TelegraphData.generated.h"

UENUM(BlueprintType)
enum class ETelegraphType : uint8
{
    Niagara    UMETA(DisplayName = "Niagara"),
    Decal      UMETA(DisplayName = "Decal"),
    UMG        UMETA(DisplayName = "UMG"),
    Custom     UMETA(DisplayName = "Custom")
};

/**
 * 텔레그래프 데이터의 추상 부모 클래스.
 * 이 클래스는 Abstract로 선언되어 블루프린트에서 직접 인스턴스화할 수 없으며,
 * 오직 자식 클래스들만 인스턴스화하여 사용해야 합니다.
 */
UCLASS(Abstract, EditInlineNew, BlueprintType, DefaultToInstanced)
class DREAMRAID_API UTelegraphData : public UObject
{
    GENERATED_BODY()

public:
    UTelegraphData();

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Telegraph")
    ETelegraphType TelegraphType;
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Telegraph")
    void StartTelegraph(FVector Location, FRotator Rotation);
    virtual void StartTelegraph_Implementation(FVector Location, FRotator Rotation) PURE_VIRTUAL(UTelegraphData::StartTelegraph_Implementation, );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Telegraph")
    void StopTelegraph();
    virtual void StopTelegraph_Implementation() PURE_VIRTUAL(UTelegraphData::StopTelegraph_Implementation, );
};
