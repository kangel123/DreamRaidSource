#include "SoundEffectAsset.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

USoundEffectAsset::USoundEffectAsset()
    : SoundFX(nullptr)
    , LifeSpan(0.f)
{
}

UObject* USoundEffectAsset::StartEffect(AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    UAudioComponent* AudioComp = nullptr;

    if (bReuseExistingComponent && PreExistingComponent)
    {
        AudioComp = Cast<UAudioComponent>(PreExistingComponent);
        if (AudioComp)
        {
            if (SoundFX)
            {
                AudioComp->SetSound(SoundFX);
            }

           if (OwnerActor && OwnerActor->GetRootComponent())
            {
                if (ComponentAttachSocket != NAME_None)
                {
                    AudioComp->AttachToComponent(OwnerActor->GetRootComponent(),
                        FAttachmentTransformRules::SnapToTargetNotIncludingScale, ComponentAttachSocket);
                }
                else
                {
                    AudioComp->AttachToComponent(OwnerActor->GetRootComponent(),
                        FAttachmentTransformRules::SnapToTargetNotIncludingScale);
                }
            }

           AudioComp->SetRelativeLocation(CustomLocation);
            AudioComp->SetRelativeRotation(CustomRotation);

            AudioComp->Play();
            return AudioComp;
        }
    }

    AudioComp = NewObject<UAudioComponent>(this);
    if (AudioComp)
    {
        AudioComp->RegisterComponentWithWorld(World);

        if (SoundFX)
        {
            AudioComp->SetSound(SoundFX);
        }

        if (OwnerActor && OwnerActor->GetRootComponent())
        {
            if (ComponentAttachSocket != NAME_None)
            {
                AudioComp->AttachToComponent(OwnerActor->GetRootComponent(),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale, ComponentAttachSocket);
            }
            else
            {
                AudioComp->AttachToComponent(OwnerActor->GetRootComponent(),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            }
        }
        else
        {
            AudioComp->SetWorldLocation(CustomLocation);
            AudioComp->SetWorldRotation(CustomRotation);
        }

        AudioComp->SetRelativeLocation(CustomLocation);
        AudioComp->SetRelativeRotation(CustomRotation);

        AudioComp->Play();
        return AudioComp;
    }

    return nullptr;
}

void USoundEffectAsset::StopEffect(UObject* EffectComponent)
{
    if (EffectComponent)
    {
        if (UAudioComponent* AudioComp = Cast<UAudioComponent>(EffectComponent))
        {
            AudioComp->Stop();
            AudioComp->DestroyComponent();
        }
    }
}
