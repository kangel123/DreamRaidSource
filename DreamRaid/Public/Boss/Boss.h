// Boss.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossStatComponent.h"
// #include "BossEnrageComponent.h"
#include "BossSkillManager.h"
#include "Boss.generated.h"

UCLASS()
class DREAMRAID_API ABoss : public ACharacter
{
    GENERATED_BODY()

public:
    ABoss();

    virtual void BeginPlay() override;

    // 스텟 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
    UBossStatComponent* StatComponent;

    // 광폭화 컴포넌트
 //   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Enrage")
 //   UBossEnrageComponent* EnrageComponent;

    // 스킬 관리 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Skills")
    UBossSkillManager* SkillManager;
};
