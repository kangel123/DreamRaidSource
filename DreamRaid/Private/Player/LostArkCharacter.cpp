#include "LostArkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h" // 로그 출력용

ALostArkCharacter::ALostArkCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 스탯 및 스킬 컴포넌트 생성
    PlayerStats = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("PlayerStatsComponent"));
    SkillManager = CreateDefaultSubobject<UPlayerSkillManagerComponent>(TEXT("SkillManagerComponent"));

    // TopDown 카메라 구성
    CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    CameraArm->SetupAttachment(RootComponent);
    CameraArm->TargetArmLength = 800.f; // 카메라와 플레이어 사이 거리
    CameraArm->bDoCollisionTest = false;
    CameraArm->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f)); // 위에서 내려다보는 시점

    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCameraComponent"));
    TopDownCameraComponent->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false;

    CurrentState = ELostArkCharacterState::Normal;
    CurrentSkillData = nullptr;
}

void ALostArkCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (PlayerStats)
    {
        PlayerStats->InitializeStats();
    }
}

USkillData* ALostArkCharacter::GetActiveSkillData_Implementation() const
{
    // 현재 활성 스킬 데이터를 반환; 예를 들어, SkillManager가 CurrentSkill을 관리한다면:
    return (SkillManager ? SkillManager->GetCurrentSkill() : CurrentSkillData);
}

void ALostArkCharacter::MoveToDestination(const FVector& Destination)
{
    // 입력된 Destination으로 이동: AddMovementInput을 이용하여 자연스러운 이동 구현
    FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction, 1.0f);
}

void ALostArkCharacter::SetCharacterState(ELostArkCharacterState NewState)
{
    CurrentState = NewState;
    UE_LOG(LogTemp, Log, TEXT("Character state changed to: %d"), (uint8)NewState);
}

// ========== IDeBuffInterface 구현 ==========

void ALostArkCharacter::ApplyDebuff_Implementation(EDebuffType DebuffType, float Duration, float TickInterval)
{
    UE_LOG(LogTemp, Log, TEXT("Applying Debuff: %d for %f sec, TickInterval: %f"), (uint8)DebuffType, Duration, TickInterval);
    if (DebuffType == EDebuffType::Freeze)
    {
        SetCharacterState(ELostArkCharacterState::Stunned);
    }
}

void ALostArkCharacter::ResetDebuffTimer_Implementation(EDebuffType DebuffType, float NewDuration)
{
    UE_LOG(LogTemp, Log, TEXT("Resetting Debuff Timer for Debuff: %d to %f sec"), (uint8)DebuffType, NewDuration);
}

bool ALostArkCharacter::IsUnderDebuff_Implementation(EDebuffType DebuffType) const
{
    return false;
}

void ALostArkCharacter::RemoveDebuff_Implementation(EDebuffType DebuffType)
{
    UE_LOG(LogTemp, Log, TEXT("Removing Debuff: %d"), (uint8)DebuffType);
    SetCharacterState(ELostArkCharacterState::Normal);
}

float ALostArkCharacter::GetDebuffRemainingTime_Implementation(EDebuffType DebuffType) const
{
    return 0.0f;
}

void ALostArkCharacter::UpdateDebuffEffect_Implementation(EDebuffType DebuffType)
{
    UE_LOG(LogTemp, Log, TEXT("Updating Debuff Effect for Debuff: %d"), (uint8)DebuffType);
}

void ALostArkCharacter::OnDebuffExpired_Implementation(EDebuffType DebuffType)
{
    UE_LOG(LogTemp, Log, TEXT("Debuff Expired: %d"), (uint8)DebuffType);
    SetCharacterState(ELostArkCharacterState::Normal);
}
