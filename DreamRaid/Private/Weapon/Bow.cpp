#include "Bow.h"
#include "Arrow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ABow::ABow()
{
    PrimaryActorTick.bCanEverTick = false;

    // 활 메쉬 생성 및 루트 설정
    BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
    RootComponent = BowMesh;

    // 기본 스폰 소켓 이름
    ArrowSpawnSocket = TEXT("ArrowSocket");
}

void ABow::BeginPlay()
{
    Super::BeginPlay();
}

AArrow* ABow::SpawnArrowFromSkill(const FBossSkillData& SkillData)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // 화살 정보가 없는 경우
    TSubclassOf<AActor> ArrowActorClass = SkillData.ClassRef;
    if (!ArrowActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkillData의 ClassRef가 설정되지 않았습니다."));
        return nullptr;
    }

    // 화살의 스폰 위치 및 회전 정보 지정
    FVector SpawnLocation = BowMesh->GetSocketLocation(ArrowSpawnSocket);
    FRotator SpawnRotation = BowMesh->GetSocketRotation(ArrowSpawnSocket);

    // 화살 액터를 스폰하고 소켓에 부착합니다.
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    // 화살은 일단 소켓에 부착된 상태로 스폰됩니다.
    AArrow* SpawnedArrow = World->SpawnActor<AArrow>(ArrowActorClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (SpawnedArrow)
    {
        // 화살을 소켓에 부착 (AttachToComponent 사용)
        SpawnedArrow->AttachToComponent(BowMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ArrowSpawnSocket);
    }

    return SpawnedArrow;
}

void ABow::FireSpawnedArrow(AArrow* Arrow, const FVector& FireDirection, float FireSpeed, float InDamage)
{
    if (!Arrow)
    {
        UE_LOG(LogTemp, Warning, TEXT("FireSpawnedArrow: 전달된 화살이 유효하지 않습니다."));
        return;
    }

    Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    Arrow->FireArrow(FireDirection, FireSpeed, InDamage);
}
