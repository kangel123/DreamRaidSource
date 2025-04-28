#include "CustomTriggerBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseCollisionShape.h"

DEFINE_LOG_CATEGORY(LogCustomTrigger);

ACustomTriggerBase::ACustomTriggerBase()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
    PrimaryActorTick.bCanEverTick = false;
    TargetActorClass = APawn::StaticClass();
}

void ACustomTriggerBase::OnConstruction(const FTransform& Transform)
{
    RebuildCollisionComponent();
}

void ACustomTriggerBase::BeginPlay()
{
    Super::BeginPlay();
    RefreshTargetActors();
    EvaluateTrigger();
}

bool ACustomTriggerBase::AreAllTargetsInside() const
{
    for (AActor* A : TargetActors)
    {
        if (!IsActorInside(A)) return false;
    }
    return TargetActors.Num() > 0;  // 타겟이 정해져 있지 않을 수도 있으므로
}

bool ACustomTriggerBase::IsAnyoneInside() const
{
    for (AActor* A : TargetActors)
    {
        if (IsActorInside(A)) return true;
    }
    return false;
}

void ACustomTriggerBase::RefreshTargetActors()
{
    TargetActors.Empty();

    if (!TargetActorClass) return;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetActorClass, Found);

    // 모든 액터에 대해서
    for (AActor* A : Found)
    {
        // 트리거 내에 태그가 정해져 있지 않거나 특정 태그를 보유하고 있는 경우
        if (RequiredTargetTag == NAME_None || A->ActorHasTag(RequiredTargetTag))
        {
            TargetActors.Add(A);
            WasInsideMap.Add(A, false);
        }
    }

    // 로그
    UE_LOG(LogCustomTrigger, Log, TEXT("[%s] Found %d target actors."),
           *GetName(), TargetActors.Num());
}

void ACustomTriggerBase::RebuildCollisionComponent()
{
    if (CollisionComp)
    {
        CollisionComp->DestroyComponent();
        CollisionComp = nullptr;
    }

    switch (ShapeType)
    {
    case ETriggerShapeType::Box:
    {
        UBoxComponent* Box = NewObject<UBoxComponent>(this, TEXT("BoxCollision"));
        Box->InitBoxExtent(BoxExtent);
        CollisionComp = Box;
        break;
    }
    case ETriggerShapeType::Sphere:
    {
        USphereComponent* Sphere = NewObject<USphereComponent>(this, TEXT("SphereCollision"));
        Sphere->InitSphereRadius(SphereRadius);
        CollisionComp = Sphere;
        break;
    }
    case ETriggerShapeType::Capsule:
    {
        UCapsuleComponent* Cap = NewObject<UCapsuleComponent>(this, TEXT("CapsuleCollision"));
        Cap->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
        CollisionComp = Cap;
        break;
    }
    case ETriggerShapeType::Custom:
        break;
    }

    if (!CollisionComp)
    {
        return;
    }

    UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(CollisionComp);
    Prim->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Prim->SetGenerateOverlapEvents(true);
    Prim->SetCollisionObjectType(ECC_WorldDynamic);
    Prim->SetCollisionResponseToAllChannels(ECR_Ignore);
    Prim->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    Prim->OnComponentBeginOverlap.AddDynamic(this, &ACustomTriggerBase::OnTriggerBeginOverlap);
    Prim->OnComponentEndOverlap.AddDynamic(this, &ACustomTriggerBase::OnTriggerEndOverlap);

    CollisionComp->SetupAttachment(RootComponent);
    CollisionComp->RegisterComponent();
}

bool ACustomTriggerBase::IsActorInside(AActor* Actor) const
{
    if (!IsValid(Actor)) return false;

    if (ShapeType == ETriggerShapeType::Custom && CustomCollisionShape)
    {
        return CustomCollisionShape->IsOverlapping(Actor, GetActorLocation(), GetActorRotation());
    }

    return CollisionComp && CollisionComp->IsOverlappingActor(Actor);
}

void ACustomTriggerBase::EvaluateTrigger() {
    
    const bool bAnyInside = IsAnyoneInside();

    if (bAnyInside && !bWasAnyInside)
    {
        StopFX(FX_OnNoneInside);
        RunFX(FX_OnAnyEnter, GetActorLocation(), GetActorRotation());
    }
    else if (!bAnyInside && bWasAnyInside)
    {
        StopFX(FX_OnAnyEnter);
        RunFX(FX_OnNoneInside, GetActorLocation(), GetActorRotation());
    }
    bWasAnyInside = bAnyInside;
}

void ACustomTriggerBase::OnTriggerBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 Idx,
                                               bool bSweep, const FHitResult& Hit)
{
    // 대상 배열에 있으면 상태 true 로
    if (WasInsideMap.Contains(OtherActor))
    {
        WasInsideMap[OtherActor] = true;
        RunFX(FX_OnActorEnter, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
        EvaluateTrigger();
    }
}

void ACustomTriggerBase::OnTriggerEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 Idx)
{
    if (WasInsideMap.Contains(OtherActor))
    {
        WasInsideMap[OtherActor] = false;
        RunFX(FX_OnActorExit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
        EvaluateTrigger();
    }
    
}

void ACustomTriggerBase::RunFX(UEffectAssetBase* FXData, FVector Location, FRotator Rotation)
{
    if (FXData)
    {
        UObject* Handle = FXData->StartEffect(this, Location, Rotation);
        if (Handle)
        {
            ActiveEffectHandles.Add(FXData, Handle);
        }
    }
}

void ACustomTriggerBase::StopFX(UEffectAssetBase* FXData, UObject* EffectHandle)
{
    if (EffectHandle)
    {
        FXData->StopEffect(EffectHandle);
        ActiveEffectHandles.Remove(FXData);
        return;
    }
    
    if (UObject** StoredHandle = ActiveEffectHandles.Find(FXData))
    {
        FXData->StopEffect(*StoredHandle);
        ActiveEffectHandles.Remove(FXData);
    }
}
