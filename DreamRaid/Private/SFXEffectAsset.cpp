// SFXEffectAsset.cpp
#include "SFXEffectAsset.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

USFXEffectAsset::USFXEffectAsset()
{
    Sound = nullptr;
}

UObject* USFXEffectAsset::StartEffect_Implementation(AActor* Owner, FVector Location, FRotator Rotation)
{
    if (!Sound || !Owner) return nullptr;

    FVector SpawnLocation  = Location;
    FRotator SpawnRotation = Rotation;
    if (SoundSettings.bUseOffset)
    {
        SpawnLocation  += SoundSettings.LocationOffset;
        SpawnRotation += SoundSettings.RotationOffset;
    }

    UAudioComponent* AudioComp = nullptr;
    switch (Attachment.AttachType)
    {
        case EEffectAttachType::ToComponent:
        case EEffectAttachType::ToSocket:
            // SpawnSoundAttached 의 네 번째 파라미터가 LocationOffset 이므로,
            // 여기에 SoundSettings.LocationOffset 을 줄 수도 있습니다.
            AudioComp = UGameplayStatics::SpawnSoundAttached(
                Sound,
                Attachment.ParentComponent,
                (Attachment.AttachType == EEffectAttachType::ToSocket)
                    ? Attachment.AttachSocketName
                    : NAME_None, SoundSettings.LocationOffset, EAttachLocation::KeepRelativeOffset, true, SoundSettings.VolumeMultiplier, SoundSettings.PitchMultiplier
            );
            break;

        case EEffectAttachType::ToActor:
        case EEffectAttachType::None:
        default:
            AudioComp = UGameplayStatics::SpawnSoundAtLocation(
                Owner->GetWorld(),
                Sound,
                SpawnLocation,
                SpawnRotation,
                SoundSettings.VolumeMultiplier,
                SoundSettings.PitchMultiplier
            );
            break;
    }

    // 2) 루핑 + 최대 지속시간 관리
    if (AudioComp && Lifetime.bLooping && Lifetime.MaxDuration > 0.0f)
    {
        FTimerDelegate Del;
        Del.BindUFunction(this, FName("StopEffect"), AudioComp);
        // 임시로 Handle 을 new 로 생성했지만, 실제로는 멤버 핸들로 관리하세요.
        FTimerHandle* Handle = new FTimerHandle();
        Owner->GetWorldTimerManager().SetTimer(*Handle, Del, Lifetime.MaxDuration, false);
    }

    return AudioComp;
}

void USFXEffectAsset::StopEffect_Implementation(UObject* EffectHandle)
{
    if (UAudioComponent* AudioComp = Cast<UAudioComponent>(EffectHandle))
    {
        AudioComp->Stop();
        if (Lifetime.bLooping)
        {
            AudioComp->DestroyComponent();
        }
    }
}
