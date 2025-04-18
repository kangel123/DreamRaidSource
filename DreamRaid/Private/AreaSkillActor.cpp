#include "AreaSkillActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AAreaSkillActor::AAreaSkillActor()
    : DamageAmount(10.f)
    , DamageTickInterval(1.f)
    , bCollisionActive(false)
{
    PrimaryActorTick.bCanEverTick = false;

    // 충돌 컴포넌트 생성 및 기본 설정
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // Overlap 이벤트 연결
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAreaSkillActor::OnOverlapBegin);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AAreaSkillActor::OnOverlapEnd);
}

void AAreaSkillActor::BeginPlay()
{
    Super::BeginPlay();
    // BeginPlay에서 추가 초기화가 필요하면 구현
}

void AAreaSkillActor::ActivateCollision(float CollisionRadius)
{
    if (!CollisionComp)
    {
        return;
    }

    CollisionComp->SetSphereRadius(CollisionRadius);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    bCollisionActive = true;
    OverlappingActors.Empty();

    // 타이머 시작: DamageTickInterval 간격으로 ApplyDamageTick 호출
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(DamageTimerHandle, this, &AAreaSkillActor::ApplyDamageTick, DamageTickInterval, true);
    }

    UE_LOG(LogTemp, Log, TEXT("AreaSkillActor: Collision activated with radius %f"), CollisionRadius);
}

void AAreaSkillActor::DeactivateCollision()
{
    if (CollisionComp)
    {
        CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    bCollisionActive = false;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(DamageTimerHandle);
    }
    OverlappingActors.Empty();

    UE_LOG(LogTemp, Log, TEXT("AreaSkillActor: Collision deactivated."));
}

void AAreaSkillActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != GetOwner())
    {
        OverlappingActors.AddUnique(OtherActor);
        UE_LOG(LogTemp, Log, TEXT("AreaSkillActor: %s entered area."), *OtherActor->GetName());
    }
}

void AAreaSkillActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != GetOwner())
    {
        OverlappingActors.Remove(OtherActor);
        UE_LOG(LogTemp, Log, TEXT("AreaSkillActor: %s left area."), *OtherActor->GetName());
    }
}

void AAreaSkillActor::ApplyDamageTick()
{
    for (AActor* Target : OverlappingActors)
    {
        if (Target)
        {
            UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, this, nullptr);
            UE_LOG(LogTemp, Log, TEXT("AreaSkillActor: Applied %f damage to %s"), DamageAmount, *Target->GetName());
        }
    }
}
