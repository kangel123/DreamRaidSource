#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

/**
 * 열거형: 기본 체력바 색상 (빨간색과 검은색은 예약되어 있음)
 */
UENUM(BlueprintType)
enum class EHealthBarColor : uint8
{
    Blue     UMETA(DisplayName = "Blue"),
    Green    UMETA(DisplayName = "Green"),
    Yellow   UMETA(DisplayName = "Yellow"),
    Orange   UMETA(DisplayName = "Orange"),
    Purple   UMETA(DisplayName = "Purple")
};

/**
 * 체력바의 한 세그먼트를 나타내는 구조체
 */
USTRUCT(BlueprintType)
struct FHealthSegment
{
    GENERATED_BODY()

    // 세그먼트의 채워진 비율 (0~1)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health Segment")
    float FillPercentage;

    // 이 세그먼트의 현재 색상
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health Segment")
    EHealthBarColor CurrentColor;

    // 이 세그먼트의 배경 색상 (다음 세그먼트의 현재 색상과 동일)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health Segment")
    EHealthBarColor BackgroundColor;

    FHealthSegment()
        : FillPercentage(1.f)
        , CurrentColor(EHealthBarColor::Blue)
        , BackgroundColor(EHealthBarColor::Blue)
    {}
};

/**
 * 보스 관련 UI를 관리하는 위젯 클래스.
 * 체력바를 여러 세그먼트로 표시하며, 나중에 다른 UI 요소도 추가할 수 있습니다.
 */
UCLASS()
class DREAMRAID_API UBossWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 주어진 세그먼트 개수만큼 체력바 세그먼트를 생성합니다.
    UFUNCTION(BlueprintCallable, Category="Boss UI")
    void GenerateSegments(int32 NumSegments);

    // 전체 체력 비율(0~1)을 받아서 각 세그먼트의 채워진 비율을 업데이트합니다.
    UFUNCTION(BlueprintCallable, Category="Boss UI")
    void UpdateHealthBar(float HealthFraction);

    // 생성된 세그먼트 배열 반환 (UI 바인딩에 활용)
    UFUNCTION(BlueprintCallable, Category="Boss UI")
    const TArray<FHealthSegment>& GetHealthSegments() const { return HealthSegments; }

protected:
    // 체력바 세그먼트 배열
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Boss UI")
    TArray<FHealthSegment> HealthSegments;

    // 색상 순서 배열: 마지막 세그먼트는 예약 색상(빨간색/검은색)이므로, 여기에는 사용 가능한 색상만 정의합니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss UI")
    TArray<EHealthBarColor> ColorOrder;

    // 예약 색상: 마지막 세그먼트의 현재 색상은 Red, 배경은 Black.
    // 빨간색과 검은색은 다른 세그먼트에서는 사용하지 않습니다.
    static const uint8 ReservedCurrentColor_Red = 255;   // 예약: Red
    static const uint8 ReservedBackgroundColor_Black = 254; // 예약: Black
};
