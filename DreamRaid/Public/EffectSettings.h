#pragma once

#include "CoreMinimal.h"
#include "EffectSettings.generated.h"


// 부착 타입
UENUM(BlueprintType)
enum class EEffectAttachType : uint8
{
    None        UMETA(DisplayName="None"),
    ToActor     UMETA(DisplayName="Attach to Actor Root"),
    ToComponent UMETA(DisplayName="Attach to Component"),
    ToSocket    UMETA(DisplayName="Attach to Socket")
};

// 부착(Attachment) 설정
USTRUCT(BlueprintType)
struct FEffectAttachmentSettings
{
    GENERATED_BODY()

    // 부착 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attachment")
    EEffectAttachType AttachType = EEffectAttachType::None;

    // AttachType==ToComponent 일 때 부모 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attachment",  meta=(EditCondition="AttachType==EEffectAttachType::ToComponent || AttachType==EEffectAttachType::ToSocket"))
    USceneComponent* ParentComponent = nullptr;

    // AttachType==ToSocket 일 때 소켓 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attachment", meta=(EditCondition="AttachType==EEffectAttachType::ToSocket"))
    FName AttachSocketName;
};

// 재생/수명(Lifetime) 설정
USTRUCT(BlueprintType)
struct FEffectLifetimeSettings
{
    GENERATED_BODY()

    // 반복 재생 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lifetime")
    bool bLooping = false;

    // 반복 시 최대 지속 시간(초). 0 이면 StopEffect 호출까지 무제한
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lifetime", meta=(EditCondition="bLooping", ClampMin="0.0"))
    float MaxDuration = 0.0f;
};


// VFX 전용 Transform 설정
USTRUCT(BlueprintType)
struct FEffectVFXSettings
{
    GENERATED_BODY()

    // 위치 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    FVector LocationOffset = FVector::ZeroVector;

    // 회전 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    FRotator RotationOffset = FRotator::ZeroRotator;
        
    // 월드 스케일
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    FVector Scale = FVector(1.f,1.f,1.f);
};

// Sound 전용 Audio 설정 (볼륨/피치 등)
USTRUCT(BlueprintType)
struct FEffectSoundSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
    bool bUseOffset = false;
    
    // 위치 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta=(EditCondition="bUseOffset==true"))
    FVector LocationOffset = FVector::ZeroVector;

    // 회전 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta=(EditCondition="bUseOffset==true"))
    FRotator RotationOffset = FRotator::ZeroRotator;
        
    
    // 사운드 볼륨 배율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta=(ClampMin="0.0", UIMin="0.0", UIMax="2.0"))
    float VolumeMultiplier = 1.0f;

    // 사운드 피치 배율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta=(ClampMin="0.1", UIMin="0.1", UIMax="3.0"))
    float PitchMultiplier = 1.0f;
};
