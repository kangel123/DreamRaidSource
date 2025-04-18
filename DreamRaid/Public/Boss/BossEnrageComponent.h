#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossEnrageComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UBossEnrageComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBossEnrageComponent();

    // Enrage 모드를 시작합니다.
    UFUNCTION(BlueprintCallable, Category="Enrage")
    void StartEnrageMode();

    // Enrage 모드를 중지합니다.
    UFUNCTION(BlueprintCallable, Category="Enrage")
    void StopEnrageMode();

protected:
    virtual void BeginPlay() override;

    // Enrage 상태에서 체력 회복 등의 주기적 처리를 담당합니다.
    UFUNCTION()
    void EnrageRegenTick();

private:
    FTimerHandle EnrageTimerHandle;
    FTimerHandle EnrageRegenTimerHandle;

public:
    // 광폭화 시작 시간 (초 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enrage")
    float EnrageTime = 900.f;

    // 회복 인터벌 (초 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enrage")
    float EnrageRegenInterval = 10.f;

    // 한 회복 당 회복량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enrage")
    float EnrageRegenAmount = 5000.f;

    // 광폭화 시 재생할 사운드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enrage")
    class USoundCue* EnrageSoundCue;

    // 광폭화 시 스폰할 Niagara 시스템
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enrage")
    class UNiagaraSystem* EnrageNiagaraSystem;

    // 광폭화 시 카메라 쉐이크 효과
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enrage")
    TSubclassOf<class UCameraShakeBase> EnrageCameraShake;
};
