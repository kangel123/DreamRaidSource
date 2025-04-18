#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnActor.generated.h"

/**
 * 애님 노티파이 "AnimNotify_SpawnActor"
 * 지정된 오프셋과 회전 보정을 적용하여 액터를 소환할 위치를 계산하는 예제입니다.
 */
UCLASS()
class DREAMRAID_API UAnimNotify_SpawnActor : public UAnimNotify
{
    GENERATED_BODY()

public:
    // 소환 위치에 추가할 오프셋. (예: 캐릭터 위치에서 약간의 보정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Adjustments")
    FVector SpawnLocationOffset;

    // 소환 시 추가할 회전 보정 값
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Adjustments")
    FRotator SpawnRotationAdjustment;

    // Notify 함수: 애니메이션의 특정 시점에 호출되어 액터 소환 등의 처리를 실행
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
