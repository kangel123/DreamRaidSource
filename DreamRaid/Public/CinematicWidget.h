#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Input/Reply.h"
#include "CinematicWidget.generated.h"

/// 시네마틱 완료 시그널
DECLARE_MULTICAST_DELEGATE(FOnCinematicFinished);

UCLASS()
class DREAMRAID_API UCinematicWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** 시네마틱 애니메이션 또는 Level Sequence 재생 */
    UFUNCTION(BlueprintCallable, Category="Cinematic")
    void PlayCinematic();

    /** ESC 스킵 안내용 이미지 노출 */
    UFUNCTION(BlueprintCallable, Category="Cinematic")
    void ShowSkipImage();

    /** 시네마틱이 끝났음을 알립니다 */
    FOnCinematicFinished OnCinematicFinished;

protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

    /** UMG에서 BindWidget 해줄 Image (Skip 아이콘) */
    UPROPERTY(meta=(BindWidget))
    UImage* SkipImage;

private:
    bool bHasFinished = false;

    void Finish();
};
