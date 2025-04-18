#include "Boss.h"
#include "BossStatComponent.h"
#include "BossEnrageComponent.h"
#include "BossSkillManager.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 스텟 컴포넌트 생성
    StatComponent = CreateDefaultSubobject<UBossStatComponent>(TEXT("StatComponent"));
    // 광폭화 컴포넌트 생성
 //   EnrageComponent = CreateDefaultSubobject<UBossEnrageComponent>(TEXT("EnrageComponent"));
    // 스킬 관리 컴포넌트 생성
    SkillManager = CreateDefaultSubobject<UBossSkillManager>(TEXT("SkillManager"));
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();
}
