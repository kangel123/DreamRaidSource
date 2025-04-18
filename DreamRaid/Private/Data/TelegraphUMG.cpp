#include "TelegraphUMG.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

UTelegraphUMG::UTelegraphUMG()
{
    TelegraphType = ETelegraphType::UMG;
    UMGWidgetClass = nullptr;
}

void UTelegraphUMG::StartTelegraph_Implementation(FVector Location, FRotator Rotation)
{
    UWorld* World = GetWorld();
    if (World && UMGWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
        if (PC)
        {
            UUserWidget* Widget = CreateWidget<UUserWidget>(PC, UMGWidgetClass);
            if (Widget)
            {
                Widget->AddToViewport();
                UMGWidget = Widget;
            }
        }
    }
}

void UTelegraphUMG::StopTelegraph_Implementation()
{
    if (UMGWidget.IsValid())
    {
        UMGWidget->RemoveFromParent();
        UMGWidget = nullptr;
    }
}
