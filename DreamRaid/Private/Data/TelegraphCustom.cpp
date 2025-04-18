#include "TelegraphCustom.h"

UTelegraphCustom::UTelegraphCustom()
{
    TelegraphType = ETelegraphType::Custom;
}

void UTelegraphCustom::StartTelegraph_Implementation(FVector Location, FRotator Rotation)
{
    // Custom 타입은 기본 구현 없이 블루프린트 오버라이드를 요구합니다.
}

void UTelegraphCustom::StopTelegraph_Implementation()
{
    // Custom 타입은 기본 구현 없이 블루프린트 오버라이드를 요구합니다.
}
