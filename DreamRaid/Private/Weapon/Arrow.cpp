#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "DeBuffInterface.h"  // 디버프 관련 인터페이스

AArrow::AArrow()
{
    PrimaryActorTick.bCanEverTick = false;

    // ArrowMesh 초기화 및 루트 설정
    ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
    RootComponent = ArrowMesh;

    // ProjectileMovement 초기화
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bAutoActivate = false;
    ProjectileMovement->bRotationFollowsVelocity = true;

    // 기본 값 설정
    ArrowType = EArrowType::Normal;
    // 기존의 ArrowAttribute 대신 DebuffType을 사용 (기본값은 None 혹은 원하는 값으로 설정)
    DebuffType = EDebuffType::None;
    FinalDamageValue = 10.f;
    IsFire = false;

    TelegraphFXComponent = nullptr;
}

void AArrow::BeginPlay()
{
    Super::BeginPlay();

    // 타입에 따른 충돌 설정 및 이벤트 바인딩
    if (ArrowType == EArrowType::Penetrating)
    {
        ArrowMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
        ArrowMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnArrowOverlap);
    }
    else
    {
        ArrowMesh->SetCollisionResponseToAllChannels(ECR_Block);
        ArrowMesh->OnComponentHit.AddDynamic(this, &AArrow::OnArrowHit);
    }
}

void AArrow::FireArrow(const FVector& ShootDirection, float Speed, float InDamage)
{
    FinalDamageValue = InDamage;
    if (ProjectileMovement)
    {
        IsFire = true;
        ProjectileMovement->Velocity = ShootDirection.GetSafeNormal() * Speed;
        ProjectileMovement->Activate();
    }

    // 화살 발사 FX (인덱스: 0-None, 1-Burn, 2-Freeze, 3-Darkness)
    int32 AttributeIndex = static_cast<int32>(DebuffType);
    if (ArrowFlightFXArray.IsValidIndex(AttributeIndex) && ArrowFlightFXArray[AttributeIndex])
    {
        UNiagaraFunctionLibrary::SpawnSystemAttached(ArrowFlightFXArray[AttributeIndex], ArrowMesh, FName("ArrowTipSocket"),
                                                      FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FX not found for DebuffType %d"), AttributeIndex);
    }
       
    // 발사 사운드
    if (FireSoundCue)
    {
        UGameplayStatics::SpawnSoundAttached(FireSoundCue, ArrowMesh, FName("ArrowTipSocket"));
    }
}

void AArrow::OnArrowHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == GetOwner())
    {
        return;
    }
    
    if (!IsFire)
    {
        return;
    }

    bool bValid = false;
    for (const FName& Tag : ValidHitTags)
    {
        if (OtherActor->ActorHasTag(Tag))
        {
            bValid = true;
            break;
        }
    }
    if (!bValid)
    {
        return;
    }
    
    // 데미지 적용
    UGameplayStatics::ApplyDamage(OtherActor, FinalDamageValue, GetInstigatorController(), this, nullptr);
    ApplyArrowAttributeEffect(OtherActor, Hit.ImpactPoint);
    Destroy();
}

void AArrow::OnArrowOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == GetOwner())
    {
        UE_LOG(LogTemp, Warning, TEXT("Ignored overlap with %s"), OtherActor ? *OtherActor->GetName() : TEXT("null"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Overlap detected with %s"), *OtherActor->GetName());

    bool bValid = false;
    for (const FName& Tag : ValidHitTags)
    {
        if (OtherActor->ActorHasTag(Tag))
        {
            bValid = true;
            break;
        }
    }
    if (!bValid)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor %s does not have a valid tag"), *OtherActor->GetName());
        return;
    }

    if (AlreadyHitActors.Contains(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Already hit %s"), *OtherActor->GetName());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Applying damage to %s"), *OtherActor->GetName());
    UGameplayStatics::ApplyDamage(OtherActor, FinalDamageValue, GetInstigatorController(), this, nullptr);
    ApplyArrowAttributeEffect(OtherActor, SweepResult.ImpactPoint);
    AlreadyHitActors.Add(OtherActor);
}
void AArrow::ApplyArrowAttributeEffect(AActor* Target, const FVector& ImpactPoint)
{
    // 속성(디버프) 타입에 따라 개별 디버프 효과 적용
    switch (DebuffType)
    {
        case EDebuffType::Burn:
            ApplyFireDebuff(Target, ImpactPoint);
            break;
        case EDebuffType::Freeze:
            ApplyIceDebuff(Target, ImpactPoint);
            break;
        case EDebuffType::Darkness:
            ApplyDebuffMaterialEffect(Target, ImpactPoint);
            break;
        default:
            break;
    }
    
    // 적중 FX (인덱스: 0-None, 1-Burn, 2-Freeze, 3-Darkness)
    int32 AttributeIndex = static_cast<int32>(DebuffType);
    if (ArrowHitFXArray.IsValidIndex(AttributeIndex) && ArrowHitFXArray[AttributeIndex])
    {
        FVector ImpactDir = (ImpactPoint - GetActorLocation()).GetSafeNormal();
        FRotator Rot = ImpactDir.Rotation();
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ArrowHitFXArray[AttributeIndex], ImpactPoint, Rot);
    }

    // 적중 사운드
    if (HitSoundCue)
    {
        UGameplayStatics::SpawnSoundAttached(HitSoundCue, ArrowMesh, FName("ArrowTipSocket"));
    }
}

void AArrow::ApplyFireDebuff(AActor* Target, const FVector& ImpactPoint)
{
    IDeBuffInterface* DebuffTarget = Cast<IDeBuffInterface>(Target);
    if (DebuffTarget)
    {
        // Burn 디버프 처리
        if (DebuffTarget->IsUnderDebuff(EDebuffType::Burn))
        {
            DebuffTarget->ResetDebuffTimer(EDebuffType::Burn, 3.f);
        }
        else
        {
            DebuffTarget->ApplyDebuff(EDebuffType::Burn, 3.f, 1.f);
        }
    }
}

void AArrow::ApplyIceDebuff(AActor* Target, const FVector& ImpactPoint)
{
    IDeBuffInterface* DebuffTarget = Cast<IDeBuffInterface>(Target);
    if (DebuffTarget)
    {
        // Freeze 디버프 처리
        if (DebuffTarget->IsUnderDebuff(EDebuffType::Freeze))
        {
            DebuffTarget->ResetDebuffTimer(EDebuffType::Freeze, 3.f);
        }
        else
        {
            DebuffTarget->ApplyDebuff(EDebuffType::Freeze, 3.f, 1.f);
        }
    }
}

void AArrow::ApplyDebuffMaterialEffect(AActor* Target, const FVector& ImpactPoint)
{
    IDeBuffInterface* DebuffTarget = Cast<IDeBuffInterface>(Target);
    if (DebuffTarget)
    {
        // Darkness 디버프 처리
        if (DebuffTarget->IsUnderDebuff(EDebuffType::Darkness))
        {
            DebuffTarget->ResetDebuffTimer(EDebuffType::Darkness, 3.f);
        }
        else
        {
            DebuffTarget->ApplyDebuff(EDebuffType::Darkness, 3.f, 1.f);
        }
    }
}
