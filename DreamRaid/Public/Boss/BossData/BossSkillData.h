#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BossSkillData.generated.h"

UENUM(BlueprintType)
enum class EBossSkillType : uint8
{
    Projectile   UMETA(DisplayName = "Projectile"),
    Melee        UMETA(DisplayName = "Melee"),
    Buff         UMETA(DisplayName = "Buff")
};

UENUM(BlueprintType)
enum class EProjectileSpawnLocationType : uint8
{
    Actor       UMETA(DisplayName = "Actor"),
    Specified   UMETA(DisplayName = "Specified"),
    Random      UMETA(DisplayName = "Random")
};

UENUM(BlueprintType)
enum class EProjectileDirectionType : uint8
{
    TowardPlayer   UMETA(DisplayName = "Toward Player"),
    Specified      UMETA(DisplayName = "Specified"),
    Random         UMETA(DisplayName = "Random")
};

USTRUCT(BlueprintType)
struct FBossSkillData : public FTableRowBase
{
    GENERATED_BODY()

    // 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    FName SkillName;

    // 쿨다운
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    float Cooldown = 5.0f;

    // 일회용 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    bool bOnceOnly = false;
    
    // 최소 사용 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    float MinRange = 0.f;
    
    // 최대 사용 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    float MaxRange = 1000.f;
    
    // 중요도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    int32 SkillPriority = 0;
    
    // 사용 확률
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    float UsageChance = 1.0f;
    
    // 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    EBossSkillType SkillType = EBossSkillType::Projectile;
    
    // 데미지 비율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    float DamageRate = 1.0f;
    
    // 몽타주 (전체 경로와 이름 포함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    FString Montage;
    
    // 딜레이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    float DelayTime = 0.f;
    
    // 텔레그래프 FX 주소 (전체 경로와 이름 포함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    FString TelegraphFX;
    
    // 관련 액터 (예: 화살 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill")
    TSubclassOf<AActor> ClassRef;

    // 투사체 속도 (투사체 타입일 때만 표시)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill", meta=(EditCondition="SkillType==EBossSkillType::Projectile"))
    float ProjectileSpeed = 2000.f;

    // 투사체 스폰 위치 타입 (투사체 타입일 때만 표시)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill", meta=(EditCondition="SkillType==EBossSkillType::Projectile"))
    EProjectileSpawnLocationType ProjectileSpawnLocationType = EProjectileSpawnLocationType::Actor;

    // 'Specified' 타입일 경우 사용할 위치
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill",
              meta=(EditCondition="SkillType==EBossSkillType::Projectile && ProjectileSpawnLocationType==EProjectileSpawnLocationType::Specified"))
    FVector SpecifiedSpawnLocation;

    // 투사체 방향 판정 (투사체 타입일 때만 표시)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill", meta=(EditCondition="SkillType==EBossSkillType::Projectile"))
    EProjectileDirectionType ProjectileDirection = EProjectileDirectionType::TowardPlayer;

    // 'Specified' 타입일 경우 날아갈 방향
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill",
              meta=(EditCondition="SkillType==EBossSkillType::Projectile && ProjectileDirection==EProjectileDirectionType::Specified"))
    FVector SpecifiedDirection;
    
    // 동시에 날아갈 화살 수 (투사체 타입일 때만 표시)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss Skill",
              meta=(EditCondition="SkillType==EBossSkillType::Projectile", ClampMin="1"))
    int32 SpreadCount = 1;
    
    // 마지막 사용 시간 (내부 관리용)
    float LastUsedTime = -999.f;
};
