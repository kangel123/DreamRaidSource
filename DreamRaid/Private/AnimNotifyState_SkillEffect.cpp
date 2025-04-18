#include "AnimNotifyState_SkillEffect.h"
#include "SkillUser.h"
#include "SkillData.h"
#include "GameFramework/Actor.h"

void UAnimNotifyState_SkillEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp)
    {
        return;
    }
    
    // 액터 확인
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }
    
    if (!Owner->GetClass()->ImplementsInterface(USkillUser::StaticClass()))
    {
        return;
    }
    
    // 현재 스킬 데이터 가져오기
    USkillData* CurrentSkill = ISkillUser::Execute_GetActiveSkillData(Owner);
    if (!CurrentSkill)
    {
        return;
    }

    // 오프셋 조정
    FVector EffectLocation = Owner->GetActorLocation() + LocationOffset;
    FRotator EffectRotation = Owner->GetActorRotation() + RotationOffset;
    
    // 효과 출력
    CurrentSkill->TriggerEffect(EffectIndex, Owner, EffectLocation, EffectRotation);
}

void UAnimNotifyState_SkillEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp)
    {
        return;
    }
    
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }
    
    if (!Owner->GetClass()->ImplementsInterface(USkillUser::StaticClass()))
    {
        return;
    }
    
    USkillData* CurrentSkill = ISkillUser::Execute_GetActiveSkillData(Owner);
    if (!CurrentSkill)
    {
        return;
    }
    
    // Notify 종료 시 해당 효과 종료
    CurrentSkill->EndEffect(EffectIndex);
}
