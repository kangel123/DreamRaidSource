#include "SkillData.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"


USkillData::USkillData()
    : SkillName(NAME_None)
    , Cooldown(1.0f)
    , DamageRate(1.0f)
    , Montage(nullptr)
    , SpawnableActorClass(nullptr)
    , SpawnLocationType(ESkillSpawnLocation::UserLocation)
    , SpawnOffset(FVector::ZeroVector)
    , SkillCategory(ESkillCategory::Custom)
    , bIsOnCooldown(false)
{
}

void USkillData::PostInitProperties()
{
    Super::PostInitProperties();
    // 추가 초기화가 필요하면 이곳에 작성
}

void USkillData::BeginCooldown()
{
    if (Cooldown <= 0.f)
    {
        bIsOnCooldown = false;
        return;
    }
    
    bIsOnCooldown = true;
    if (UWorld* World = GetWorld())
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            bIsOnCooldown = false;
            UE_LOG(LogTemp, Log, TEXT("Skill %s: Cooldown ended."), *SkillName.ToString());
        }, Cooldown, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Skill %s: GetWorld() returned null."), *SkillName.ToString());
    }
}

void USkillData::ExecuteSkill_Implementation(AActor* SkillOwner, const FVector& ProvidedTargetLocation)
{
    // 1. 애니메이션 몽타주 실행 (필요한 경우)
    if (Montage && SkillOwner)
    {
        if (USkeletalMeshComponent* MeshComp = SkillOwner->FindComponentByClass<USkeletalMeshComponent>())
        {
            if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
            {
                UE_LOG(LogTemp, Log, TEXT("Playing montage %s"), *Montage->GetName());
                float MontageDuration = AnimInstance->Montage_Play(Montage);
                if (MontageDuration <= 0.f)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Montage Play failed for montage %s"), *Montage->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No AnimInstance found on %s"), *SkillOwner->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No SkeletalMeshComponent found on %s"), *SkillOwner->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Montage is null or SkillOwner is null."));
    }
    
    // 2. BeginCooldown 호출하여 이후 재사용 대기 시간 시작
    BeginCooldown();
    
    // 3. SpawnableActorClass가 지정되었다면 스폰 로직 실행
    if (SpawnableActorClass)
    {
        FVector FinalSpawnLocation = ProvidedTargetLocation;
        APlayerController* PC = UGameplayStatics::GetPlayerController(SkillOwner, 0);

        // SpawnLocationType에 따라 스폰 위치를 결정
        switch (SpawnLocationType)
        {
            case ESkillSpawnLocation::MouseLocation:
            {
                if (PC)
                {
                    FHitResult HitResult;
                    if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
                    {
                        FinalSpawnLocation = HitResult.ImpactPoint;
                    }
                    else
                    {
                        FinalSpawnLocation = SkillOwner->GetActorLocation();
                    }
                }
                else
                {
                    FinalSpawnLocation = SkillOwner->GetActorLocation();
                }
                break;
            }
            case ESkillSpawnLocation::MouseDirection:
            {
                if (PC)
                {
                    FVector2D MousePosition;
                    if (PC->GetMousePosition(MousePosition.X, MousePosition.Y))
                    {
                        FVector WorldOrigin, WorldDirection;
                        if (PC->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldOrigin, WorldDirection))
                        {
                            // 플레이어 위치에서 WorldDirection 방향으로 SpawnOffset.X (예: 거리) 만큼 이동한 위치 계산
                            FinalSpawnLocation = SkillOwner->GetActorLocation() + WorldDirection * SpawnOffset.X;
                        }
                        else
                        {
                            FinalSpawnLocation = SkillOwner->GetActorLocation();
                        }
                    }
                    else
                    {
                        FinalSpawnLocation = SkillOwner->GetActorLocation();
                    }
                }
                else
                {
                    FinalSpawnLocation = SkillOwner->GetActorLocation();
                }
                break;
            }
            case ESkillSpawnLocation::UserLocation:
            default:
                FinalSpawnLocation = SkillOwner->GetActorLocation();
                break;
        }
        
        // 추가 보정값 추가 (예: Z축 보정 등)
        FinalSpawnLocation += SpawnOffset;
        
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = SkillOwner;
        SpawnParams.Instigator = SkillOwner->GetInstigator();

        // AActor 타입으로 액터를 소환합니다.
        AActor* SpawnedActor = SkillOwner->GetWorld()->SpawnActor<AActor>(
            SpawnableActorClass,
            FinalSpawnLocation,
            FRotator::ZeroRotator,
            SpawnParams
        );
        
        if (SpawnedActor)
        {
            // 스폰된 액터에 대해 추가 초기화가 필요하면 여기에 추가합니다.
            UE_LOG(LogTemp, Log, TEXT("Spawned actor at location: %s"), *FinalSpawnLocation.ToString());
        }
    }
}

void USkillData::CancelSkill_Implementation(AActor* SkillOwner, float BlendOutTime)
{
    if (Montage && SkillOwner)
    {
        if (USkeletalMeshComponent* MeshComp = SkillOwner->FindComponentByClass<USkeletalMeshComponent>())
        {
            if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
            {
                // BlendOutTime 0.2초로 Montage를 강제 중단
                AnimInstance->Montage_Stop(BlendOutTime, Montage);
                UE_LOG(LogTemp, Log, TEXT("Skill %s: Montage stopped."), *SkillName.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("CancelSkill: No AnimInstance found on %s"), *SkillOwner->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("CancelSkill: No SkeletalMeshComponent found on %s"), *SkillOwner->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CancelSkill: Montage is null or SkillOwner is null."));
    }
}

void USkillData::StartTelegraphs_Implementation(AActor* SkillOwner, const FVector& Location, const FRotator& Rotation)
{
    for (UTelegraphData* Telegraph : TelegraphDatas)
    {
        if (Telegraph)
        {
            Telegraph->StartTelegraph(Location, Rotation);
        }
    }
}

void USkillData::StopTelegraphs_Implementation()
{
    for (UTelegraphData* Telegraph : TelegraphDatas)
    {
        if (Telegraph)
        {
            Telegraph->StopTelegraph();
        }
    }
}

void USkillData::TriggerEffect(int32 EffectIndex, AActor* OwnerActor, FVector CustomLocation, FRotator CustomRotation)
{
    if (!EffectAssets.IsValidIndex(EffectIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("TriggerEffect: Invalid EffectIndex %d"), EffectIndex);
        return;
    }
    
    UEffectAssetBase* Effect = EffectAssets[EffectIndex];
    if (!Effect)
    {
        return;
    }
    
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("TriggerEffect: OwnerActor is null."));
        return;
    }

    UObject* SpawnedInstance = Effect->StartEffect(OwnerActor, CustomLocation, CustomRotation);
    if (SpawnedInstance)
    {
        TArray<UObject*>& InstanceArray = RunningEffectInstances.FindOrAdd(EffectIndex);
        InstanceArray.Add(SpawnedInstance);
    }
}

void USkillData::EndEffect(int32 EffectIndex)
{
    UE_LOG(LogTemp, Log, TEXT("EndEffect called with index: %d"), EffectIndex);
    if (!EffectAssets.IsValidIndex(EffectIndex))
    {
        return;
    }
    
    UEffectAssetBase* Effect = EffectAssets[EffectIndex];
    if (!Effect)
    {
        return;
    }
    
    if (TArray<UObject*>* InstanceArrayPtr = RunningEffectInstances.Find(EffectIndex))
    {
        for (UObject* Instance : *InstanceArrayPtr)
        {
            if (Instance)
            {
                Effect->StopEffect(Instance);
            }
        }
        RunningEffectInstances.Remove(EffectIndex);
    }
}


