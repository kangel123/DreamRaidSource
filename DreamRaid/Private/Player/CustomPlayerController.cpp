#include "CustomPlayerController.h"
#include "LostArkCharacter.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

ACustomPlayerController::ACustomPlayerController()
{
    bHasValidTarget = false;
    bIsRightClickHeld = false;
    TargetLocation = FVector::ZeroVector;
    bShowMouseCursor = true;
}

void ACustomPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("RightClick", IE_Pressed, this, &ACustomPlayerController::OnRightClickPressed);
        InputComponent->BindAction("RightClick", IE_Released, this, &ACustomPlayerController::OnRightClickReleased);
        InputComponent->BindAction("BasicAttack", IE_Pressed, this, &ACustomPlayerController::OnBasicAttackPressed);
        InputComponent->BindAction("Dodge", IE_Pressed, this, &ACustomPlayerController::OnDodgePressed);

        // 일반 스킬 입력
        InputComponent->BindAction("NormalSkillQ", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillQPressed);
        InputComponent->BindAction("NormalSkillW", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillWPressed);
        InputComponent->BindAction("NormalSkillE", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillEPressed);
        InputComponent->BindAction("NormalSkillR", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillRPressed);
        InputComponent->BindAction("NormalSkillA", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillAPressed);
        InputComponent->BindAction("NormalSkillS", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillSPressed);
        InputComponent->BindAction("NormalSkillD", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillDPressed);
        InputComponent->BindAction("NormalSkillF", IE_Pressed, this, &ACustomPlayerController::OnNormalSkillFPressed);
    }
}

void ACustomPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ProcessMoveToDestination();
}

void ACustomPlayerController::OnRightClickPressed()
{
    bIsRightClickHeld = true;
}

void ACustomPlayerController::OnRightClickReleased()
{
    bIsRightClickHeld = false;
}


void ACustomPlayerController::ProcessMoveToDestination()
{
    // 우클릭이 눌린 상태이면 계속 업데이트
    if (bIsRightClickHeld)
    {
        FHitResult Hit;
        if (GetHitResultUnderCursor(ECC_Visibility, false, Hit) && Hit.bBlockingHit)
        {
            TargetLocation = Hit.Location;
            bHasValidTarget = true;
        }
    }

    if (!bHasValidTarget)
    {
        return;
    }

    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        // 상태가 Normal일 때만 이동
        if (MyCharacter->CurrentState != ELostArkCharacterState::Normal)
        {
            return;
        }
        float Distance = FVector::Dist(MyCharacter->GetActorLocation(), TargetLocation);
        const float Tolerance = 3.f;
        if (Distance > Tolerance)
        {
            MyCharacter->MoveToDestination(TargetLocation);
        }
        else
        {
            bHasValidTarget = false;
        }
    }
}

void ACustomPlayerController::OnBasicAttackPressed()
{
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        // 기본 공격은 키 바인딩이 좌클릭로 SkillManager에 등록된 스킬을 실행합니다.
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::LeftMouseButton);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnDodgePressed()
{
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        // 회피는 스페이스바에 바인딩된 스킬 실행
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::SpaceBar);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillQPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill Q Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::Q);
            
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillWPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill W Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::W);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillEPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill E Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::E);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillRPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill R Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::R);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillAPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill A Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::A);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillSPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill S Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::S);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillDPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill D Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::D);
        }
        bHasValidTarget = false;
    }
}

void ACustomPlayerController::OnNormalSkillFPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Normal Skill F Pressed"));
    if (ALostArkCharacter* MyCharacter = Cast<ALostArkCharacter>(GetPawn()))
    {
        if (MyCharacter->SkillManager)
        {
            MyCharacter->SkillManager->ExecuteSkillByKey(EKeys::F);
        }
        bHasValidTarget = false;
    }
}
