#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"
#include "SkillRanged.generated.h"

/**
 * 발사 방향 타입
 */
UENUM(BlueprintType)
enum class ELaunchDirectionType : uint8
{
    SpecificTarget UMETA(DisplayName = "Specific Target"), // OptionalTargetLocation 사용
    ActorForward   UMETA(DisplayName = "Actor Forward"),    // ProjectileSpawnComponent 또는 SkillOwner의 전방
    Random         UMETA(DisplayName = "Random"),           // 랜덤 방향
    Custom         UMETA(DisplayName = "Custom")            // CustomLaunchDirection 사용
};

/**
 * 원거리 스킬 전용 클래스.
 * 투사체 기반 스킬에 필요한 파라미터와, 투사체를 먼저 부착된 상태로 스폰한 후 나중에 발사하는 기능을 제공합니다.
 */
UCLASS(EditInlineNew, BlueprintType)
class DREAMRAID_API USkillRanged : public USkillData
{
    GENERATED_BODY()

public:
    USkillRanged();

    /** 투사체 클래스 (예: AArrow) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged")
    TSubclassOf<AActor> ProjectileClass;

    /** 투사체 속도 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged")
    float ProjectileSpeed;

    /** 투사체 수명 (초) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged")
    float ProjectileLifetime;

    /** 기본 투사체 스폰 오프셋 (SkillOwner 기준) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged")
    FVector ProjectileSpawnOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Ranged")
    USceneComponent* ProjectileSpawnComponent;


    /** 한 번의 스킬 사용에 발사할 투사체 수 (1 이상) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged", meta = (ClampMin = "1"))
    int32 SpreadCount;

    /** 투사체 간 확산 각도 (각 투사체에 적용할 Yaw 오프셋) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged")
    float SpreadAngle;

    /** 발사 방향 타입 선택 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged")
    ELaunchDirectionType LaunchDirectionType;

    /** 사용자 지정 발사 방향 (Custom 타입 사용 시) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged", meta = (EditCondition = "LaunchDirectionType==ELaunchDirectionType::Custom"))
    FVector CustomLaunchDirection;

    /**
     * 투사체를 스폰하고 발사하는 함수.
     * OptionalTargetLocation이 유효하면, 해당 위치를 타겟으로 하는 방향으로 투사체를 발사합니다.
     */
    UFUNCTION(BlueprintCallable, Category = "Ranged")
    void FireProjectile(AActor* SkillOwner, const FVector& OptionalTargetLocation);

    /**
     * 투사체를 스폰하는 함수.
     * 스폰 시, ProjectileSpawnComponent가 지정되어 있으면 그 위치를 사용하고,
     * 그렇지 않으면 SkillOwner의 위치에 ProjectileSpawnOffset을 더한 위치에서 스폰합니다.
     */
    UFUNCTION(BlueprintCallable, Category = "Ranged")
    AActor* SpawnProjectile(AActor* SkillOwner);
    
    /**
     * 스폰된 투사체를 발사하는 함수.
     * 투사체를 Detach한 후, 계산된 발사 방향과 함께 초기화하여 발사합니다.
     * OptionalTargetLocation은 SpecificTarget 방식일 때 사용됩니다.
     */
    UFUNCTION(BlueprintCallable, Category = "Ranged")
    void LaunchProjectile(AActor* SkillOwner, AActor* Projectile, const FVector& OptionalTargetLocation);

protected:
    /**
     * SkillOwner의 IStatInterface를 통해 Attack 값을 가져와 최종 데미지를 계산합니다.
     * 최종 데미지는 DamageRate와 Attack의 곱입니다.
     */
    float CalculateFinalDamage(AActor* SkillOwner) const;

    /**
     * 발사 방향을 계산하는 함수.
     * - SpecificTarget: OptionalTargetLocation이 유효하면, SpawnLocation에서 OptionalTargetLocation까지의 방향을 사용.
     * - ActorForward: ProjectileSpawnComponent가 있으면 그 컴포넌트의 전방, 없으면 SkillOwner의 전방.
     * - Random: 랜덤 유닛 벡터.
     * - Custom: CustomLaunchDirection의 정규화된 벡터.
     */
    FVector CalculateLaunchDirection(AActor* SkillOwner, const FVector& SpawnLocation, const FVector& OptionalTargetLocation) const;
};
