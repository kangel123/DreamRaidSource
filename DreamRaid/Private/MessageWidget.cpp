// MessageWidget.cpp
#include "MessageWidget.h"
#include "TimerManager.h"

void UMessageWidget::SetMessage(const FText& NewMessage)
{
    if (!MessageText) return;

    MessageText->SetText(NewMessage);

    // 자동 숨김 타이머
    if (DisplayDuration > 0.f && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHideHandle);
        GetWorld()->GetTimerManager().SetTimer(AutoHideHandle, this, &UMessageWidget::HandleHide,DisplayDuration, false);
    }
}

void UMessageWidget::HideMessage()
{
    HandleHide();
}

void UMessageWidget::HandleHide()
{
    RemoveFromParent();
    OnMessageHidden.Broadcast();
}
