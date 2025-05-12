#include "CinematicWidget.h"

void UCinematicWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetKeyboardFocus();
}

FReply UCinematicWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    if (!bHasFinished && InKeyEvent.GetKey() == EKeys::Escape)
    {
        Finish();
        return FReply::Handled();
    }
    return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}

void UCinematicWidget::PlayCinematic()
{
    // BP에서 Intro 애니메이션 또는 LevelSequencePlayer 재생
}

void UCinematicWidget::ShowSkipImage()
{
    if (SkipImage)
    {
        SkipImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void UCinematicWidget::Finish()
{
    if (bHasFinished) return;
    bHasFinished = true;
    OnCinematicFinished.Broadcast();
}
