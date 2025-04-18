#include "CustomTriggerBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "BaseCollisionShape.h"
#include "Components/CapsuleComponent.h"

ACustomTriggerBase::ACustomTriggerBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACustomTriggerBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    RebuildCollisionComponent();
}

void ACustomTriggerBase::RebuildCollisionComponent()
{
    if (CollisionComp)
    {
        CollisionComp->DestroyComponent();
        CollisionComp = nullptr;
    }

    if (ShapeType == ETriggerShapeType::Custom)
    {
        // custom 모드면 별도 콜리전 x
        return;
    }

    UShapeComponent* NewComp = nullptr;
    switch (ShapeType)
    {
    case ETriggerShapeType::Box:
        {
            UBoxComponent* BoxComp = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("BoxTrigger"));
            BoxComp->InitBoxExtent(BoxExtent);
            NewComp = BoxComp;
        }
        break;
    case ETriggerShapeType::Sphere:
        {
            USphereComponent* SphereComp = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("SphereTrigger"));
            SphereComp->InitSphereRadius(SphereRadius);
            NewComp = SphereComp;
        }
        break;
    case ETriggerShapeType::Capsule:
        {
            UCapsuleComponent* CapsuleComp = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("CapsuleTrigger"));
            CapsuleComp->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
            NewComp = CapsuleComp;
        }
        break;
    default:
        break;
    }

    CollisionComp = NewComp;
    if (CollisionComp)
    {
        CollisionComp->SetCollisionProfileName(TEXT("Trigger"));
        CollisionComp->SetGenerateOverlapEvents(true);
        CollisionComp->RegisterComponent();

        if (!RootComponent)
        {
            RootComponent = CollisionComp;
        }
        else
        {
            CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void ACustomTriggerBase::BeginPlay()
{
    Super::BeginPlay();

    // 자식이 override 가능
    AutoFindActors();

    // 초기화
    for (AActor* Actor : TargetActors)
    {
        WasInsideMap.Add(Actor, false);
    }

    bHadPlayersLastFrame = false;
}

void ACustomTriggerBase::AutoFindActors_Implementation()
{
    // 기본 구현: 아무것도 안 함 -> 자식이 필요하면 override
}

void ACustomTriggerBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 커스텀 콜리전?
    if (ShapeType == ETriggerShapeType::Custom)
    {
        if (!CustomCollisionShape || TargetActors.Num() == 0) return;

        bool bAnyInside = false;

        for (AActor* Actor : TargetActors)
        {
            if (!Actor) continue;

            bool bWasInside = WasInsideMap[Actor];
            bool bIsInside = CustomCollisionShape->IsOverlapping(Actor, GetActorLocation(), GetActorRotation());

            if (!bWasInside && bIsInside)
            {
                WasInsideMap[Actor] = true;
                HandleTriggerEnter(Actor);
            }
            else if (bWasInside && !bIsInside)
            {
                WasInsideMap[Actor] = false;
                HandleTriggerExit(Actor);
            }

            if (bIsInside) bAnyInside = true;
        }

        if (bAnyInside && !bHadPlayersLastFrame)
        {
            UpdateHasPlayersFX(true);
        }
        else if (!bAnyInside && bHadPlayersLastFrame)
        {
            UpdateHasPlayersFX(false);
        }
        bHadPlayersLastFrame = bAnyInside;

        // 예: 조건 만족 시점에 OnTriggerActivated()를 호출해도 됨
        // 자식이 override해서 사용
    }
    else
    {
        // box / sphere / capsule
        if (!CollisionComp || TargetActors.Num() == 0) return;

        TArray<AActor*> Overlaps;
        CollisionComp->GetOverlappingActors(Overlaps);

        bool bAnyInside = false;
        for (AActor* Actor : TargetActors)
        {
            if (!Actor) continue;

            bool bWasInside = WasInsideMap[Actor];
            bool bIsInside = Overlaps.Contains(Actor);

            if (!bWasInside && bIsInside)
            {
                WasInsideMap[Actor] = true;
                HandleTriggerEnter(Actor);
            }
            else if (bWasInside && !bIsInside)
            {
                WasInsideMap[Actor] = false;
                HandleTriggerExit(Actor);
            }
            if (bIsInside) bAnyInside = true;
        }

        if (bAnyInside && !bHadPlayersLastFrame)
        {
            UpdateHasPlayersFX(true);
        }
        else if (!bAnyInside && bHadPlayersLastFrame)
        {
            UpdateHasPlayersFX(false);
        }
        bHadPlayersLastFrame = bAnyInside;
    }
}

void ACustomTriggerBase::HandleTriggerEnter(AActor* Actor)
{
    // 자식이 필요하면 override
}

void ACustomTriggerBase::HandleTriggerExit(AActor* Actor)
{
    // 자식이 필요하면 override
}

void ACustomTriggerBase::UpdateHasPlayersFX_Implementation(bool bAnyInside)
{
    // 기본 구현 없음
}

void ACustomTriggerBase::OnTriggerActivated_Implementation()
{
    // 자식 override
}
