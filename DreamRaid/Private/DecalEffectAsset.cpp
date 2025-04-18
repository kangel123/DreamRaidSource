#include "DecalEffectAsset.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UDecalEffectAsset::UDecalEffectAsset()
    : DecalMaterial(nullptr)
    , DecalSize(FVector(200.f, 200.f, 200.f)) // 기본 데칼 크기 지정
    , LifeSpan(0.f)                          // 0이면 영구적으로 유지됨
{
    // 생성자에서 필요한 기본값들을 초기화합니다.
}

UObject* UDecalEffectAsset::StartEffect(AActor* OwnerActor, FVector CustomLocation, FRotator CustomRotation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    UDecalComponent* DecalComp = nullptr;

    // 이미 존재하는 컴포넌트를 재사용하는 경우 처리
    if (bReuseExistingComponent && PreExistingComponent)
    {
        DecalComp = Cast<UDecalComponent>(PreExistingComponent);
        if (DecalComp)
        {
            // 데칼 속성 업데이트
            if (DecalMaterial)
            {
                DecalComp->SetDecalMaterial(DecalMaterial);
            }
            DecalComp->DecalSize = DecalSize;

            // 소켓 이름이 유효하면 해당 소켓에 부착, 아니면 액터의 RootComponent에 부착
            if (OwnerActor && OwnerActor->GetRootComponent())
            {
                if (ComponentAttachSocket != NAME_None)
                {
                    DecalComp->AttachToComponent(OwnerActor->GetRootComponent(),
                        FAttachmentTransformRules::SnapToTargetNotIncludingScale, ComponentAttachSocket);
                }
                else
                {
                    DecalComp->AttachToComponent(OwnerActor->GetRootComponent(),
                        FAttachmentTransformRules::SnapToTargetNotIncludingScale);
                }
            }
            
            // 부착 후 상대적 위치와 회전값 설정 (소켓 부착 시 오프셋 역할로 사용)
            DecalComp->SetRelativeLocation(CustomLocation);
            DecalComp->SetRelativeRotation(CustomRotation);

            // LifeSpan 설정 (0보다 크면 제한된 수명)
            if (LifeSpan > 0.f)
            {
                DecalComp->SetLifeSpan(LifeSpan);
            }
            
            DecalComp->SetVisibility(true);
            return DecalComp;
        }
    }

    // 새 데칼 컴포넌트 생성
    DecalComp = NewObject<UDecalComponent>(this);
    if (DecalComp)
    {
        // 월드에 컴포넌트를 등록해야 화면에 나타납니다.
        DecalComp->RegisterComponentWithWorld(World);
        
        // 데칼 속성 설정
        if (DecalMaterial)
        {
            DecalComp->SetDecalMaterial(DecalMaterial);
        }
        DecalComp->DecalSize = DecalSize;
        
        // 소켓 이름 유무에 따른 부착 처리
        if (OwnerActor && OwnerActor->GetRootComponent())
        {
            if (ComponentAttachSocket != NAME_None)
            {
                DecalComp->AttachToComponent(OwnerActor->GetRootComponent(),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale, ComponentAttachSocket);
            }
            else
            {
                DecalComp->AttachToComponent(OwnerActor->GetRootComponent(),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            }
        }
        else
        {
            // 소유 액터가 없으면 월드 좌표 기준으로 위치와 회전 설정
            DecalComp->SetWorldLocation(CustomLocation);
            DecalComp->SetWorldRotation(CustomRotation);
        }
        
        if (LifeSpan > 0.f)
        {
            DecalComp->SetLifeSpan(LifeSpan);
        }
        return DecalComp;
    }
    
    return nullptr;
}

void UDecalEffectAsset::StopEffect(UObject* EffectComponent)
{
    if (EffectComponent)
    {
        if (UDecalComponent* DecalComp = Cast<UDecalComponent>(EffectComponent))
        {
            // 데칼 컴포넌트 제거
            DecalComp->DestroyComponent();
        }
    }
}
