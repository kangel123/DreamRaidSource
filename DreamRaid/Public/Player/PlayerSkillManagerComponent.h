#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSkillData.h"
#include "PlayerSkillManagerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UPlayerSkillManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerSkillManagerComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FPlayerSkillData> PlayerSkills;

    // 현재 활성 스킬 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
    class USkillData* CurrentSkill;

    UFUNCTION(BlueprintCallable, Category = "Skill")
    void ExecuteSkillByKey(const FKey& PressedKey);

    UFUNCTION(BlueprintCallable, Category = "Skill")
    USkillData* GetCurrentSkill() const { return CurrentSkill; }

    UFUNCTION(BlueprintCallable, Category = "Skill")
    void ResetCurrentSkill();
    
    UFUNCTION(BlueprintCallable, Category = "Skill")
    AActor* SpawnSkillActor(TSubclassOf<AActor> ActorToSpawn, const FVector& Location, const FRotator& Rotation);
};
