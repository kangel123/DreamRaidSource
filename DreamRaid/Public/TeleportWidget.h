#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "TeleportWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCinematicFinished);

UCLASS()
class DREAMRAID_API UTeleportWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 시네마틱 애니메이션 재생
    UFUNCTION(BlueprintCallable)
    void PlayTeleportAnimation();

    // 스킵 버튼 보이기
    UFUNCTION(BlueprintCallable)
    void ShowSkipButton();

    // 페이드아웃 시작
    UFUNCTION(BlueprintCallable)
    void StartFadeOut();

    // 시네마틱 완료 콜백
    FOnCinematicFinished OnCinematicFinished;

protected:
    virtual void NativeConstruct() override;

    // UMG 바인딩
    UPROPERTY(meta = (BindWidget))
    UButton* SkipButton;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutAnim;

private:
    bool bHasStartedFade = false;

    UFUNCTION()
    void HandleSkipClicked();

    UFUNCTION()
    void HandleFadeOutFinished();
};
