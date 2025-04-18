#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SkillEffect.generated.h"

UCLASS()
class DREAMRAID_API UAnimNotifyState_SkillEffect : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    // 실행할 효과 에셋 배열 인덱스 (예: 0부터 시작)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillEffect")
    int32 EffectIndex;

    // 효과 실행 시 사용할 위치 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillEffect")
    FVector LocationOffset;

    // 효과 실행 시 사용할 회전 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillEffect")
    FRotator RotationOffset;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
