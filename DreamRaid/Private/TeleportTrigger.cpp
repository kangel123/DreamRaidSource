#include "TeleportTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "TeleportableInterface.h"

ATeleportTrigger::ATeleportTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATeleportTrigger::EvaluateTrigger()
{
    Super::EvaluateTrigger();
    if (bTeleportInProgress) return;

    int32 Inside=0, Total=WasInsideMap.Num();
    for (auto& P:WasInsideMap) if (P.Value) ++Inside;

    if (Total>0 && Inside==Total)
    {
        if (!bCountdownActive)
            StartCountdown();
    }
    else if (Inside==0 && bCountdownActive)
    {
        CancelCountdown();
    }
}

void ATeleportTrigger::StartCountdown()
{
    bCountdownActive   = true;
    RemainingCountdown = CountdownTime;

    if (MessageWidgetClass && GetWorld())
    {
        if (auto PC = GetWorld()->GetFirstPlayerController())
        {
            ActiveMessageWidget = CreateWidget<UMessageWidget>(PC, MessageWidgetClass);
            ActiveMessageWidget->AddToViewport();
            
        }
        
    }

    if (ActiveMessageWidget)
    {
        ActiveMessageWidget->SetMessage(
            FText::Format(
                NSLOCTEXT("Teleport","Countdown","{0}초 후에 자동으로 이동됩니다."),
                FText::AsNumber(FMath::CeilToInt(RemainingCountdown))
            )
        );
    }

    GetWorldTimerManager().SetTimer(
        CountdownTickHandle, this, &ATeleportTrigger::UpdateCountdownTick, 1.f, true
    );
    GetWorldTimerManager().SetTimer(
        CountdownHandle, this, &ATeleportTrigger::OnCountdownFinished, CountdownTime, false
    );
}

void ATeleportTrigger::CancelCountdown()
{
    bCountdownActive = false;
    GetWorldTimerManager().ClearTimer(CountdownTickHandle);
    GetWorldTimerManager().ClearTimer(CountdownHandle);

    if (ActiveMessageWidget)
    {
        ActiveMessageWidget->HideMessage();
        ActiveMessageWidget = nullptr;
    }
}

void ATeleportTrigger::UpdateCountdownTick()
{
    RemainingCountdown -= 1.f;
    if (ActiveMessageWidget && RemainingCountdown > 0.f)
    {
        ActiveMessageWidget->SetMessage(
            FText::Format(
                NSLOCTEXT("Teleport","Countdown","{0}초 후에 자동으로 이동됩니다."),
                FText::AsNumber(FMath::CeilToInt(RemainingCountdown))
            )
        );
    }
}

void ATeleportTrigger::OnCountdownFinished()
{
    bCountdownActive = false;
    GetWorldTimerManager().ClearTimer(CountdownTickHandle);

    if (ActiveMessageWidget)
    {
        ActiveMessageWidget->SetMessage(NSLOCTEXT("Teleport","Notice","잠시 후 이동됩니다."));
    }

    GetWorldTimerManager().SetTimer(
        NoticeHandle, this, &ATeleportTrigger::StartTeleportSequence,
        NoticeDisplayTime, false
    );
}

void ATeleportTrigger::StartTeleportSequence()
{
    bTeleportInProgress = true;

    if (auto PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
    }

    for (auto& P:WasInsideMap)
        if (P.Key && P.Key->GetClass()->ImplementsInterface(UTeleportableInterface::StaticClass()))
            ITeleportableInterface::Execute_PrepareForTeleport(P.Key);

    MoveActorsToCenter();

    GetWorldTimerManager().SetTimer(
        InitialFadeHandle, this, &ATeleportTrigger::OnInitialFadeComplete,
        DelayAfterCenter, false
    );
}

void ATeleportTrigger::MoveActorsToCenter()
{
    FVector C = GetActorLocation();
    for (auto& P:WasInsideMap)
        if (auto A=P.Key) A->SetActorLocation(C,false);
}

void ATeleportTrigger::OnInitialFadeComplete()
{
    OnTeleportStart.Broadcast();

    TeleportActorsToDestination();
    OnTeleportArrived.Broadcast();

    if (CinematicWidgetClass && GetWorld())
    {
        if (auto PC = GetWorld()->GetFirstPlayerController())
        {
            ActiveCinematicWidget = CreateWidget<UCinematicWidget>(PC, CinematicWidgetClass);
            ActiveCinematicWidget->AddToViewport();
            ActiveCinematicWidget->PlayCinematic();
            ActiveCinematicWidget->ShowSkip();
            ActiveCinematicWidget->OnCinematicFinished.AddUObject(
                this, &ATeleportTrigger::StartFinalFade
            );
        }
    }
    else
    {
        StartFinalFade();
    }
}

void ATeleportTrigger::TeleportActorsToDestination()
{
    if (DestinationType==ETeleportDestinationType::AnotherMap)
    {
        if (!DestinationMapName.IsNone())
            UGameplayStatics::OpenLevel(this, DestinationMapName);
    }
    else
    {
        for (auto& P:WasInsideMap)
            if (auto A=P.Key) A->SetActorLocation(DestinationLocation,false);
    }
}

void ATeleportTrigger::StartFinalFade()
{
    GetWorldTimerManager().SetTimer(
        FinalFadeHandle, [this]()
        {
            OnTeleportFinish.Broadcast();
            FinishTeleportSequence();
        },
        FadeDuration, false
    );
}

void ATeleportTrigger::FinishTeleportSequence()
{
    if (auto PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
    }

    if (ActiveMessageWidget)   ActiveMessageWidget->HideMessage();
    if (ActiveCinematicWidget) ActiveCinematicWidget->RemoveFromParent();

    bTeleportInProgress = false;
}

void ATeleportTrigger::LogState(const FString& Msg) const
{
    UE_LOG(LogTemp, Log, TEXT("[%s] %s"), *GetName(), *Msg);
}
