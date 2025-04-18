#include "BossSkillManager.h"
#include "Engine/DataTable.h"
#include "Boss.h"
#include "NiagaraSystem.h"
#include "EnvironmentalEffectManager.h"
#include "Kismet/KismetMathLibrary.h"

UBossSkillManager::UBossSkillManager()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentPhaseIndex = 0;
}

void UBossSkillManager::BeginPlay()
{
    Super::BeginPlay();
    LoadSkillData();
    LoadPhaseData();
    
    // EnvironmentalEffectManager가 같은 액터에 있다면 델리게이트에 바인딩합니다.
    UEnvironmentalEffectManager* EffectManager = GetOwner()->FindComponentByClass<UEnvironmentalEffectManager>();
    if (EffectManager)
    {
        OnEnvironmentalEffectsTriggered.AddDynamic(EffectManager, &UEnvironmentalEffectManager::TriggerEnvironmentalEffects);
    }
}

void UBossSkillManager::LoadSkillData()
{
    if (SkillDataTable)
    {
        static const FString ContextString(TEXT("Boss Skill Data Context"));
        TArray<FBossSkillData*> Rows;
        SkillDataTable->GetAllRows<FBossSkillData>(ContextString, Rows);
        AllSkills.Empty();
        for (FBossSkillData* Row : Rows)
        {
            if (Row)
            {
                AllSkills.Add(*Row);
            }
        }
    }
}

void UBossSkillManager::LoadPhaseData()
{
    if (PhaseDataTable)
    {
        static const FString ContextString(TEXT("Boss Phase Data Context"));
        TArray<FBossPhaseData*> Rows;
        PhaseDataTable->GetAllRows<FBossPhaseData>(ContextString, Rows);
        AllPhases.Empty();
        for (FBossPhaseData* Row : Rows)
        {
            if (Row)
            {
                AllPhases.Add(*Row);
            }
        }
    }
}

void UBossSkillManager::UpdatePhase(float CurrentHealthPercent)
{
    int32 NewPhaseIndex = CurrentPhaseIndex;
    for (const FBossPhaseData& Phase : AllPhases)
    {
        if (Phase.TriggerCondition == EPhaseTriggerCondition::HPBased &&
            CurrentHealthPercent <= Phase.TriggerHPPercent)
        {
            NewPhaseIndex = FMath::Max(NewPhaseIndex, Phase.PhaseIndex);
        }
    }

    if (NewPhaseIndex != CurrentPhaseIndex)
    {
        CurrentPhaseIndex = NewPhaseIndex;
        const FBossPhaseData& CurrentPhase = AllPhases[CurrentPhaseIndex];

        // 몽타주가 존재하면 실행
        ABoss* BossOwner = Cast<ABoss>(GetOwner());
        if (BossOwner && !CurrentPhase.PhaseTransitionMontage.IsEmpty())
        {
            FString MontagePath = CurrentPhase.PhaseTransitionMontage;
            UAnimMontage* Montage = LoadObject<UAnimMontage>(nullptr, *MontagePath);
            if (Montage)
            {
                // GetAnimInstance()를 통해 애니메이션 인스턴스를 가져오고, Montage_Play를 호출합니다.
                if (UAnimInstance* AnimInstance = BossOwner->GetMesh()->GetAnimInstance())
                {
                    AnimInstance->Montage_Play(Montage);
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to load montage from path: %s"), *MontagePath);
            }
        }

        // 환경 효과 트리거가 존재하면 델리게이트 방송
        if (CurrentPhase.EnvironmentalEffectTriggers.Num() > 0)
        {
            OnEnvironmentalEffectsTriggered.Broadcast(CurrentPhase.EnvironmentalEffectTriggers);
        }
    }
}


FBossSkillData UBossSkillManager::ChooseSkill(float CurrentHealthPercent, float DistanceToPlayer)
{
    double CurrentTime = GetCurrentTime();

    // 먼저 페이즈를 업데이트합니다.
    UpdatePhase(CurrentHealthPercent);

    // 페이즈 내의 스킬들 가져오기
    TArray<FBossSkillData> CandidateSkills;
    for (const FBossPhaseData& Phase : AllPhases)
    {
        if (Phase.PhaseIndex == CurrentPhaseIndex)
        {
            for (const FBossSkillData& Skill : Phase.PhaseSkills)
            {
                CandidateSkills.Add(Skill);
            }
        }
    }
    
    // 만약 페이즈에 등록된 스킬이 없다면 기본 목록을 사용
    if (CandidateSkills.Num() == 0)
    {
        CandidateSkills = AllSkills;
    }
    
    // 스킬 조건들 확인
    TArray<FBossSkillData> ValidSkills;
    for (FBossSkillData Skill : CandidateSkills)
    {
        // 거리 조건
        if (DistanceToPlayer < Skill.MinRange || DistanceToPlayer > Skill.MaxRange)
        {
            continue;
        }

        // 쿨타임 조건
        float CooldownToUse = Skill.Cooldown;
        // if (bIsEnraged) { CooldownToUse *= Skill.fCooldownWhenEnragedRate; }
        if (CurrentTime - Skill.LastUsedTime < CooldownToUse)   // 현재 시간과 최근에 사용한 시간을 통해 쿨타임 여부를 결정함
        {
            continue;
        }
        
        // 사용 확률
        float RandomValue = UKismetMathLibrary::RandomFloat();
        if (RandomValue > Skill.UsageChance)
        {
            continue;
        }
        
        ValidSkills.Add(Skill);
    }
    
    // 사용가능한 스킬이 존재하는 경우
    if (ValidSkills.Num() > 0)
    {
        // 우선순위가 높은 순서대로 정렬
        ValidSkills.Sort([](const FBossSkillData& A, const FBossSkillData& B) {
            return A.SkillPriority > B.SkillPriority;
        });
        
        // 가장 높은 우선순위를 가진 스킬들 추출
        int32 TopPriority = ValidSkills[0].SkillPriority;
        TArray<FBossSkillData> TopPrioritySkills;
        for (const FBossSkillData& Skill : ValidSkills)
        {
            if (Skill.SkillPriority == TopPriority)
            {
                TopPrioritySkills.Add(Skill);
            }
            else
            {
                break;
            }
        }
        
        // 거기서 랜덤으로 스킬 하나만 사용함
        int32 RandomIndex = UKismetMathLibrary::RandomInteger(TopPrioritySkills.Num());
        FBossSkillData ChosenSkill = TopPrioritySkills[RandomIndex];
        
        // 사용한 스킬의 마지막 사용 시간을 현재 시간으로 만듦
        ChosenSkill.LastUsedTime = CurrentTime;
    
        return ChosenSkill;
    }
    
    return FBossSkillData();
}
