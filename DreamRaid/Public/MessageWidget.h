// MessageWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MessageWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMessageHidden);

UCLASS()
class DREAMRAID_API UMessageWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="UI")
    void SetMessage(const FText& NewMessage);

    /** 즉시 숨기기 */
    UFUNCTION(BlueprintCallable, Category="UI")
    void HideMessage();

    /** 메시지가 사라진 직후 브로드캐스트됩니다. */
    UPROPERTY(BlueprintAssignable, Category="UI")
    FOnMessageHidden OnMessageHidden;

protected:
    /** UMG에서 BindWidget 해둘 TextBlock */
    UPROPERTY(meta=(BindWidget))
    UTextBlock* MessageText;

    /** 자동 숨김까지 시간(초). 0 이하면 자동 숨김 안 함 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timing", meta=(ClampMin="0.0"))
    float DisplayDuration = 2.f;

private:
    FTimerHandle AutoHideHandle;
    UFUNCTION()
    void HandleHide();
};
