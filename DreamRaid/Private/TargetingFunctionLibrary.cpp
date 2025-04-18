#include "TargetingFunctionLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

AActor* UTargetingFunctionLibrary::GetActorUnderCursor(APlayerController* PlayerController, bool bDrawDebug)
{
    if (!PlayerController)
    {
        return nullptr;
    }

    FVector2D MousePosition;
    if (!PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        return nullptr;
    }

    FHitResult HitResult;
    bool bHit = PlayerController->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, HitResult);

    if (bDrawDebug && PlayerController->GetWorld())
    {
        DrawDebugSphere(PlayerController->GetWorld(), HitResult.ImpactPoint, 16.0f, 12, FColor::Green, false, 2.0f);
    }

    return HitResult.GetActor();
}

bool UTargetingFunctionLibrary::IsValidSkillTarget(AActor* TargetActor)
{
    // 단순한 null 체크 외에 조건을 추가할 수 있습니다.
    return (TargetActor != nullptr);
}
