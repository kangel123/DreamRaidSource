#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillUser.generated.h"

UINTERFACE(Blueprintable)
class DREAMRAID_API USkillUser : public UInterface
{
    GENERATED_BODY()
};

class DREAMRAID_API ISkillUser
{
    GENERATED_BODY()

public:
    // 현재 활성 스킬 데이터를 반환하는 함수
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Skill")
    class USkillData* GetActiveSkillData() const;
};
