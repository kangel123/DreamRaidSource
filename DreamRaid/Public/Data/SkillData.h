#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"
#include "EffectAssetBase.h"
#include "TelegraphData.h"
#include "GameFramework/Actor.h"
#include "SkillData.generated.h"


UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Melee    UMETA(DisplayName = "Melee"),
    Ranged   UMETA(DisplayName = "Ranged"),
    Area     UMETA(DisplayName = "Area"),
    Buff     UMETA(DisplayName = "Buff"),
    Custom   UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ESkillSpawnLocation : uint8
{
    MouseLocation   UMETA(DisplayName = "Mouse Location"),
    MouseDirection  UMETA(DisplayName = "Mouse Direction"),
    UserLocation    UMETA(DisplayName = "User Location")
};


UCLASS(Abstract, EditInlineNew, BlueprintType, DefaultToInstanced)
class DREAMRAID_API USkillData : public UObject
{
    GENERATED_BODY()

public:
    USkillData();

    // 스킬 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillName;

    // 쿨타임
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown;

    // 데미지비율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float DamageRate;

    // 몽타주
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* Montage;

    // 스폰할 액터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TSubclassOf<AActor> SpawnableActorClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillSpawnLocation SpawnLocationType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FVector SpawnOffset;
    
    // FX 효과 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TArray<UEffectAssetBase*> EffectAssets;

    // 텔레그래프 데이터 배열
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Transient, Category = "Telegraph")
    TArray<UTelegraphData*> TelegraphDatas;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    ESkillCategory SkillCategory;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    bool bIsOnCooldown;

    // 스킬 실행 함수들
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
    void ExecuteSkill(AActor* SkillOwner, const FVector& SpawnLocation);
    virtual void ExecuteSkill_Implementation(AActor* SkillOwner, const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
    void CancelSkill(AActor* SkillOwner, float BlendOutTime);
    virtual void CancelSkill_Implementation(AActor* SkillOwner, float BlendOutTime);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Telegraph")
    void StartTelegraphs(AActor* SkillOwner, const FVector& Location, const FRotator& Rotation);
    virtual void StartTelegraphs_Implementation(AActor* SkillOwner, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Telegraph")
    void StopTelegraphs();
    virtual void StopTelegraphs_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Skill")
    void TriggerEffect(int32 EffectIndex, AActor* OwnerActor,FVector CustomLocation, FRotator CustomRotation);

    UFUNCTION(BlueprintCallable, Category = "Skill")
    void EndEffect(int32 EffectIndex);

protected:
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void BeginCooldown();

    virtual void PostInitProperties() override;

private:
    TMap<int32, TArray<UObject*>> RunningEffectInstances;
};
