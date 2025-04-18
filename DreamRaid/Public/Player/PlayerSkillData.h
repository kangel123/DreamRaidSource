#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h" // FKey, EKeys
#include "SkillData.h"
#include "PlayerSkillData.generated.h"

// 스킬 종류 열거형
UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Basic           UMETA(DisplayName = "Basic"),
    Normal          UMETA(DisplayName = "Normal"),
    Identity        UMETA(DisplayName = "Identity"),
    Awakening       UMETA(DisplayName = "Awakening"),
    SuperAwakening  UMETA(DisplayName = "SuperAwakening"),
    Dodge           UMETA(DisplayName = "Dodge"),
    Rising          UMETA(DisplayName = "Rising")
};


USTRUCT(BlueprintType)
struct FPlayerSkillData
{
    GENERATED_BODY()

public:
    /** 인라인으로 생성 및 편집 가능한 USkillData 인스턴스 */
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Skill")
    USkillData* SkillInstance;

    // 스킬 종류
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;
    
    /** 해당 스킬을 실행할 키 바인딩 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FKey KeyBinding;

    FPlayerSkillData()
        : SkillInstance(nullptr)
        , SkillType(ESkillType::Basic)
        , KeyBinding(EKeys::Invalid)
    {}
    
    
};
