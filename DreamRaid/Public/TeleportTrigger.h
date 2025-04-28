#pragma once

#include "CoreMinimal.h"
#include "CustomTriggerBase.h"
#include "TeleportTrigger.generated.h"

// 텔레포트 방식
UENUM(BlueprintType)
enum class ETeleportDestinationType : uint8
{
    SameMapLocation,
    AnotherMap
};

UCLASS()
class DREAMRAID_API ATeleportTrigger : public ACustomTriggerBase
{
    GENERATED_BODY()

public:
    ATeleportTrigger();

    
    // 카운트 다운
    UPROPERTY(EditAnywhere, Category="Teleport")
    float CountdownTime = 10.f;

    // 지연 시간
    UPROPERTY(EditAnywhere, Category="Teleport")
    float DelayAfterCenter = 1.f;

    // 텔레포트 방식
    UPROPERTY(EditAnywhere, Category="Teleport")
    ETeleportDestinationType DestinationType = ETeleportDestinationType::SameMapLocation;

    // 이동할 좌표(SameMapLocation)
    UPROPERTY(EditAnywhere, Category="Teleport", meta=(EditCondition="DestinationType==ETeleportDestinationType::SameMapLocation"))
    FVector DestinationLocation;

    // 이동할 맵(AnotherMap)
    UPROPERTY(EditAnywhere, Category="Teleport", meta=(EditCondition="DestinationType==ETeleportDestinationType::AnotherMap"))
    FName DestinationMapName = NAME_None;

protected:
    virtual void EvaluateTrigger() override;    // 실행
    
private:
    bool bTeleportInProgress = false;
    bool bCountdownActive = false; // 카운트 다운 실행 플래그
    float RemainingCountdown;               // 남은 카운트다운 시간
    
    FTimerHandle CountdownHandle;   // 카운트 다운 핸들러
    FTimerHandle CountdownLogHandle;        // 로그용 반복 타이머
    FTimerHandle DelayTeleportHandle;   // 지연 시간 핸들러

    void LogCountdownTick();    // 남은 시간 로그
    void StartCountdown();  // 카운트 시작
    void CancelCountdown(); // 카운트 종료
    void OnCountdownFinished(); // 카운트 완료 시
    
    void StartTeleportSequence();   // 텔레포트 순서
    void MoveActorsToCenter();  // 모든 액터를 가운데로 이동
    void TeleportActorsToDestination(); // 목적지로 이동

    void LogState(const FString& Msg) const;    // 로그
};
