#include "VisualEffectAsset.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UVisualEffectAsset::UVisualEffectAsset()
    : VisualFX(nullptr)
    , LifeSpan(0.f)
{
}

UObject* UVisualEffectAsset::StartEffect(AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    UNiagaraComponent* NiagaraComp = nullptr;


    if (bReuseExistingComponent && PreExistingComponent)
    {
        NiagaraComp = Cast<UNiagaraComponent>(PreExistingComponent);
        if (NiagaraComp)
        {
            if (VisualFX)
            {
                NiagaraComp->SetAsset(VisualFX);
            }
            
            if (OwnerActor && OwnerActor->GetRootComponent())
            {
                if (ComponentAttachSocket != NAME_None)
                {
                    NiagaraComp->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, ComponentAttachSocket);
                }
                else
                {
                    NiagaraComp->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
                }
            }
            
            NiagaraComp->SetRelativeLocation(CustomLocation);
            NiagaraComp->SetRelativeRotation(CustomRotation);
            NiagaraComp->Activate(true);
            
            // LifeSpan이 지정된 경우 타이머로 컴포넌트를 제거하도록 예약
            if (LifeSpan > 0.f)
            {
                FTimerHandle TimerHandle;
                World->GetTimerManager().SetTimer(TimerHandle, [NiagaraComp]()
                {
                    if (NiagaraComp)
                    {
                        NiagaraComp->Deactivate();
                        NiagaraComp->DestroyComponent();
                    }
                }, LifeSpan, false);
            }
            
            return NiagaraComp;
        }
    }

    NiagaraComp = NewObject<UNiagaraComponent>(this);
    if (NiagaraComp)
    {
        NiagaraComp->RegisterComponentWithWorld(World);
        if (VisualFX)
        {
            NiagaraComp->SetAsset(VisualFX);
        }
        
        if (OwnerActor && OwnerActor->GetRootComponent())
        {
            if (ComponentAttachSocket != NAME_None)
            {
                NiagaraComp->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, ComponentAttachSocket);
            }
            else
            {
                NiagaraComp->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            }
        }
        else
        {
            NiagaraComp->SetWorldLocation(CustomLocation);
            NiagaraComp->SetWorldRotation(CustomRotation);
        }
        
        NiagaraComp->SetRelativeLocation(CustomLocation);
        NiagaraComp->SetRelativeRotation(CustomRotation);
        NiagaraComp->Activate(true);

        if (LifeSpan > 0.f)
        {
            FTimerHandle TimerHandle;
            World->GetTimerManager().SetTimer(TimerHandle, [NiagaraComp]()
            {
                if (NiagaraComp)
                {
                    NiagaraComp->Deactivate();
                    NiagaraComp->DestroyComponent();
                }
            }, LifeSpan, false);
        }

        return NiagaraComp;
    }

    return nullptr;
}

void UVisualEffectAsset::StopEffect(UObject* EffectComponent)
{
    if (EffectComponent)
    {
        if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(EffectComponent))
        {
            NiagaraComp->Deactivate();
            NiagaraComp->DestroyComponent();
        }
    }
}
