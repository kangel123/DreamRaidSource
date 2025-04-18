#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSkillData.h"
#include "Bow.generated.h"

/**
 * ABow 클래스
 * - 보스가 들고 있는 무기(활)로, 스킬 정보를 기반으로 화살을 스폰하고 발사합니다.
 * - 스폰 시 FX를 실행하여 플레이어에게 스킬 사용 전 시각적 효과를 제공합니다.
 */
UCLASS()
class DREAMRAID_API ABow : public AActor
{
    GENERATED_BODY()

public:
    ABow();

    virtual void BeginPlay() override;

    // 메쉬
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* BowMesh;

    // 화살이 스폰될 소켓 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
    FName ArrowSpawnSocket;

    // 화살 스폰
    UFUNCTION(BlueprintCallable, Category = "Bow")
    class AArrow* SpawnArrowFromSkill(const FBossSkillData& SkillData);

    // 스폰된 화살 발사
    UFUNCTION(BlueprintCallable, Category = "Bow")
    void FireSpawnedArrow(class AArrow* Arrow, const FVector& FireDirection, float FireSpeed, float InDamage);
    
};
