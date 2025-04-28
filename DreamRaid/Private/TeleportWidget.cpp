#include "TeleportWidget.h"

void UTeleportWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SkipButton)
    {
        SkipButton->SetVisibility(ESlateVisibility::Collapsed);
        SkipButton->OnClicked.AddDynamic(this, &UTeleportWidget::HandleSkipClicked);
    }
}

void UTeleportWidget::PlayTeleportAnimation()
{
    
}

void UTeleportWidget::ShowSkipButton()
{
    if (SkipButton)
    {
        SkipButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UTeleportWidget::HandleSkipClicked()
{
    StartFadeOut();
}

void UTeleportWidget::StartFadeOut()
{
    if (bHasStartedFade) return;
    bHasStartedFade = true;

    if (FadeOutAnim)
    {
        PlayAnimation(FadeOutAnim);
        FWidgetAnimationDynamicEvent EndEvent;
        EndEvent.BindDynamic(this, &UTeleportWidget::HandleFadeOutFinished);
        BindToAnimationFinished(FadeOutAnim, EndEvent);
    }
    else
    {
        HandleFadeOutFinished();
    }
}

void UTeleportWidget::HandleFadeOutFinished()
{
    OnCinematicFinished.Broadcast();
}
