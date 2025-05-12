#pragma once

#include "CoreMinimal.h"
#include "CustomTriggerBase.h"
#include "MessageWidget.h"
#include "CinematicWidget.h"
#include "TeleportTrigger.generated.h"

// ① 중앙 이동 후 시네마틱 준비 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportStart);
// ② 목적지 이동 직후 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportArrived);
// ③ 최종 페이드아웃 후 텔레포트 완료 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportFinish);

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

    /** 보스 초기화·시네마틱 준비용 */
    UPROPERTY(BlueprintAssignable, Category="Teleport|Event")
    FOnTeleportStart OnTeleportStart;

    /** 스킵 활성화 등 목적지 도착 알림용 */
    UPROPERTY(BlueprintAssignable, Category="Teleport|Event")
    FOnTeleportArrived OnTeleportArrived;

    /** 입력 복구·전투 시작 알림용 */
    UPROPERTY(BlueprintAssignable, Category="Teleport|Event")
    FOnTeleportFinish OnTeleportFinish;

    /** 카운트다운 시간(초) */
    UPROPERTY(EditAnywhere, Category="Teleport")
    float CountdownTime = 10.f;

    /** 중앙 이동 후 초기 페이드아웃 지연(초) */
    UPROPERTY(EditAnywhere, Category="Teleport")
    float DelayAfterCenter = 1.f;

    /** SameMap vs AnotherMap */
    UPROPERTY(EditAnywhere, Category="Teleport")
    ETeleportDestinationType DestinationType = ETeleportDestinationType::SameMapLocation;

    /** 같은 맵 이동 좌표 */
    UPROPERTY(EditAnywhere, Category="Teleport", meta=(EditCondition="DestinationType==ETeleportDestinationType::SameMapLocation"))
    FVector DestinationLocation;

    /** 다른 맵 이름 */
    UPROPERTY(EditAnywhere, Category="Teleport", meta=(EditCondition="DestinationType==ETeleportDestinationType::AnotherMap"))
    FName DestinationMapName = NAME_None;

    /** 범용 메시지 위젯 클래스 */
    UPROPERTY(EditAnywhere, Category="Teleport|UI")
    TSubclassOf<UMessageWidget> MessageWidgetClass;

    /** “잠시 후 이동됩니다.” 메시지 표시 시간(초) */
    UPROPERTY(EditAnywhere, Category="Teleport|UI", meta=(ClampMin="0.0"))
    float NoticeDisplayTime = 1.5f;

    /** 시네마틱 위젯 클래스 */
    UPROPERTY(EditAnywhere, Category="Teleport|UI")
    TSubclassOf<UCinematicWidget> CinematicWidgetClass;

    /** 최종 페이드아웃 지속시간(초) */
    UPROPERTY(EditAnywhere, Category="Teleport|UI", meta=(ClampMin="0.0"))
    float FadeDuration = 1.f;

protected:
    virtual void EvaluateTrigger() override;

private:
    bool bTeleportInProgress = false;
    bool bCountdownActive   = false;
    float RemainingCountdown = 0.f;

    FTimerHandle CountdownTickHandle;
    FTimerHandle CountdownHandle;
    FTimerHandle NoticeHandle;
    FTimerHandle InitialFadeHandle;
    FTimerHandle FinalFadeHandle;

    UMessageWidget*  ActiveMessageWidget   = nullptr;
    UCinematicWidget* ActiveCinematicWidget = nullptr;

    // 카운트다운
    void StartCountdown();
    void CancelCountdown();
    void UpdateCountdownTick();
    void OnCountdownFinished();

    // 텔레포트 시퀀스
    void StartTeleportSequence();
    void MoveActorsToCenter();
    void OnInitialFadeComplete();
    void TeleportActorsToDestination();
    void StartFinalFade();
    void FinishTeleportSequence();

    // (선택) 로그 유틸
    void LogState(const FString& Msg) const;
};
