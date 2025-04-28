#include "TeleportTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "TeleportableInterface.h"
#include "TimerManager.h"

ATeleportTrigger::ATeleportTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
    bTeleportInProgress = false;
    bCountdownActive    = false;
}

void ATeleportTrigger::EvaluateTrigger()
{
    Super::EvaluateTrigger();
    
    if (bTeleportInProgress) return;
    
    int32 InsideCount = 0;  // 들어간 인원 수
    int32 TotalCount = WasInsideMap.Num();  // 총 인원 수

    // 들어간 인원 수 체크
    for (auto& Pair : WasInsideMap)
    {
        if (Pair.Value)
            InsideCount++;
    }

    const bool bAllInside = (TotalCount > 0 && InsideCount == TotalCount);  // 모두 들어가 있는가?
    const bool bAnyInside = (InsideCount > 0);  // 한명이라도 들어가 있는가?

    if (bAllInside)
       {
           if (bCountdownActive) CancelCountdown();
           StartTeleportSequence();
       }
       else if (bAnyInside && !bCountdownActive)
       {
           StartCountdown();
       }
       else if (!bAnyInside && bCountdownActive)
       {
           CancelCountdown();
       }

}

void ATeleportTrigger::StartCountdown()
{
    bCountdownActive = true;    // 카운트 시작 플래그 온
    RemainingCountdown = CountdownTime; // 남은 시간
    
    LogState(TEXT("카운트 시작"));
    // 1초마다 남은 시간 로그 남기기
    GetWorldTimerManager().SetTimer(CountdownLogHandle,this,&ATeleportTrigger::LogCountdownTick,1.0f,true);

    // 타이머 완료 시 호출
    GetWorldTimerManager().SetTimer(CountdownHandle,this,&ATeleportTrigger::OnCountdownFinished,CountdownTime,false);
}

void ATeleportTrigger::CancelCountdown()
{
    bCountdownActive = false;   // 카운트 시작 플래그 오프
    GetWorldTimerManager().ClearTimer(CountdownHandle); // 카운트 핸들러 종료
    GetWorldTimerManager().ClearTimer(CountdownLogHandle);  // 카운트 로그 핸들러 종료
    LogState(TEXT("카운트 종료"));
}

void ATeleportTrigger::OnCountdownFinished()
{
    bCountdownActive = false;   // 카운트 시작 플래그 오프
    LogState(TEXT("카운트 완료"));
    StartTeleportSequence();    // 텔레포트 시작
}

void ATeleportTrigger::StartTeleportSequence()
{
    LogState(TEXT("텔레포트 시작"));
    bTeleportInProgress = true;
    
    for (auto& Pair : WasInsideMap)
    {
        if (Pair.Key && Pair.Key->GetClass()->ImplementsInterface(UTeleportableInterface::StaticClass()))
        {
            ITeleportableInterface::Execute_PrepareForTeleport(Pair.Key);
        }
    }
    
    // 중앙으로 이동
    MoveActorsToCenter();

    // 지연 후 최종 목적지로 이동
    GetWorldTimerManager().SetTimer(DelayTeleportHandle, [this]()
    {
        TeleportActorsToDestination();

        // 4) 텔레포트 완료: 인터페이스로 잠금 해제
        for (auto& Pair : WasInsideMap)
        {
            if (Pair.Key && Pair.Key->GetClass()->ImplementsInterface(UTeleportableInterface::StaticClass()))
            {
                ITeleportableInterface::Execute_FinishTeleport(Pair.Key);
            }
        }
        bTeleportInProgress = false;
    }, DelayAfterCenter, false);
}

void ATeleportTrigger::MoveActorsToCenter()
{
    const FVector Center = GetActorLocation();  // 가운데 위치 가져오기

    // 모든 액터를 가운데로 이동
    LogState(TEXT("중앙 이동"));
    for (auto& Pair : WasInsideMap)
    {
        if (AActor* Actor = Pair.Key)
        {
            Actor->SetActorLocation(Center, false);
        }
    }
}

void ATeleportTrigger::TeleportActorsToDestination()
{
    
    LogState(TEXT("목적지로 이동"));
    
    // 다른 맵으로 이동
    if (DestinationType == ETeleportDestinationType::AnotherMap)
    {
        // 비어있지 않으면
        if (!DestinationMapName.IsNone())
        {
            LogState(FString::Printf(TEXT(" 이동할 맵 : %s"), *DestinationMapName.ToString()));
            UGameplayStatics::OpenLevel(this, DestinationMapName);
        }
        else    // 비었으면
        {
            LogState(TEXT("[에러] 목적지가 지정 안됨"));
        }
    }
    else // 같은 맵에서 이동
    {
        for (auto& Pair : WasInsideMap)
        {
            if (AActor* Actor = Pair.Key)
            {
                Actor->SetActorLocation(DestinationLocation, false);
                LogState(FString::Printf(TEXT("이동할 좌표 : %s"), *DestinationLocation.ToString()));
            }
        }
    }
}

void ATeleportTrigger::LogState(const FString& Msg) const
{
    UE_LOG(LogTemp, Log, TEXT("[%s] %s"), *GetName(), *Msg);
}

void ATeleportTrigger::LogCountdownTick()
{
    RemainingCountdown -= 1.f;

    if (RemainingCountdown > 0.f)
    {
        LogState(FString::Printf(TEXT("%d초"), FMath::CeilToInt(RemainingCountdown)));
    }
    else
    {
        // 0초 되면 로그 타이머는 멈추고
        GetWorldTimerManager().ClearTimer(CountdownLogHandle);
    }
}
