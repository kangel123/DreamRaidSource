// UDeBuffInterface.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DebuffTypes.h"
#include "DeBuffInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDeBuffInterface : public UInterface
{
    GENERATED_BODY()
};

class DREAMRAID_API IDeBuffInterface
{
    GENERATED_BODY()

public:
    // 디버프 적용 (예: Burn은 도트 데미지, Freeze는 이동속도 감소, Darkness는 시야 차단)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    void ApplyDebuff(EDebuffType DebuffType, float Duration, float TickInterval);

    // 디버프 지속 시간 갱신
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    void ResetDebuffTimer(EDebuffType DebuffType, float NewDuration);

    // 대상이 특정 디버프 상태인지 확인
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    bool IsUnderDebuff(EDebuffType DebuffType) const;

    // 디버프 제거 (수동으로 디버프를 해제)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    void RemoveDebuff(EDebuffType DebuffType);

    // 디버프의 남은 지속 시간을 반환 (UI 업데이트 등에서 활용)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    float GetDebuffRemainingTime(EDebuffType DebuffType) const;

    // 디버프 효과를 업데이트 (예: TickInterval마다 도트 데미지 적용 등)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    void UpdateDebuffEffect(EDebuffType DebuffType);

    // 디버프가 만료되었을 때 호출 (종료 후 후처리용)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debuff")
    void OnDebuffExpired(EDebuffType DebuffType);
};
