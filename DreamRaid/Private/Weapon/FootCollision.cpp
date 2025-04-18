#include "FootCollision.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UFootCollision::UFootCollision()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetCollisionObjectType(ECC_WorldDynamic);
    SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void UFootCollision::BeginPlay()
{
    Super::BeginPlay();

    // 캡슐 컴포넌트의 Overlap 이벤트에 HandleOverlapBegin 바인딩
    OnComponentBeginOverlap.AddDynamic(this, &UFootCollision::HandleOverlapBegin);
}

void UFootCollision::HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                         bool bFromSweep, const FHitResult & SweepResult)
{
    // 맞은 대상이 존재하는 지 확인
    if (!OtherActor || OtherActor == GetOwner())
    {
        return;
    }
    
    // 태그 확인
    bool bValid = false;
    for (const FName& Tag : ValidHitTags)
    {
        if (OtherActor->ActorHasTag(Tag))
        {
            bValid = true;
            break;
        }
    }

    // 맞을 수 있는 태그가 없다면 무시
    if (!bValid)
    {
           return;
    }
    
    // 이미 맞은 대상이면 무시
    if (AlreadyHitActors.Contains(OtherActor))
    {
        return;
    }


    ApplyDamageToTarget(OtherActor, DamageAmount);

    // 대상이 ACharacter인 경우 에어본 효과 적용
    ACharacter* TargetChar = Cast<ACharacter>(OtherActor);
    if (TargetChar)
    {
        ApplyAirborneEffect(OtherActor, AirborneLaunchVelocity, AirborneReducedGravity, AirborneDuration, AirControlWhileAirborne);
        AlreadyHitActors.Add(OtherActor);
    }
}

void UFootCollision::ApplyDamageToTarget(AActor* Target, float InDamageAmount)
{
    if (Target)
    {
        UGameplayStatics::ApplyDamage(Target, InDamageAmount, GetOwner()->GetInstigatorController(), GetOwner(), nullptr);
    }
}

void UFootCollision::ApplyAirborneEffect(AActor* Target, const FVector& LaunchVelocity, float ReducedGravityScale, float EffectDuration, float AirControlValue)
{
    // 맞은 대상 가져오기
    ACharacter* TargetChar = Cast<ACharacter>(Target);
    if (TargetChar)
    {
        // 이동컴포넌트 가져오기
        if (UCharacterMovementComponent* MoveComp = TargetChar->GetCharacterMovement())
        {
            float OriginalGravity = MoveComp->GravityScale; // 기존 중력
            float OriginalAirControl = MoveComp->AirControl;    // 기존 공중 조작

            // 중력 변경
            MoveComp->GravityScale = ReducedGravityScale;
            MoveComp->AirControl = AirControlValue;
            TargetChar->LaunchCharacter(LaunchVelocity, true, true);

            // 일정 시간이 지난 후에 원래 중력과 공중 조작으로 변경
            FTimerHandle RestoreTimer;
            GetWorld()->GetTimerManager().SetTimer(RestoreTimer, [MoveComp, OriginalGravity, OriginalAirControl]()
            {
                if (MoveComp)
                {
                    MoveComp->GravityScale = OriginalGravity;
                    MoveComp->AirControl = OriginalAirControl;
                }
            }, EffectDuration, false);
        }
    }
}

void UFootCollision::ResetHitTargets()
{
    AlreadyHitActors.Empty();
}

void UFootCollision::EnableFootCollision()
{
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UFootCollision::DisableFootCollision()
{
    SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
