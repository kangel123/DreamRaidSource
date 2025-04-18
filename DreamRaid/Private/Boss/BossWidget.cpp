#include "BossWidget.h"
#include "Kismet/KismetMathLibrary.h"

void UBossWidget::GenerateSegments(int32 NumSegments)
{
    HealthSegments.Empty();

    // 최소 1개 이상의 세그먼트가 필요합니다.
    if (NumSegments < 1)
    {
        return;
    }

    // 마지막 세그먼트를 제외한 나머지 생성
    for (int32 i = 0; i < NumSegments - 1; ++i)
    {
        FHealthSegment Segment;

        // ColorOrder 배열에 따라 현재 색상과 배경 색상을 할당합니다.
        if (ColorOrder.Num() > 0)
        {
            Segment.CurrentColor = ColorOrder[i % ColorOrder.Num()];
            Segment.BackgroundColor = ColorOrder[(i + 1) % ColorOrder.Num()];
        }
        else
        {
            // ColorOrder가 비어있으면 기본값으로 Blue 사용
            Segment.CurrentColor = EHealthBarColor::Blue;
            Segment.BackgroundColor = EHealthBarColor::Blue;
        }
        Segment.FillPercentage = 1.f;
        HealthSegments.Add(Segment);
    }

    // 마지막 세그먼트: 예약된 색상 사용 (현재: Red, 배경: Black)
    FHealthSegment LastSegment;
    LastSegment.CurrentColor = static_cast<EHealthBarColor>(ReservedCurrentColor_Red);
    LastSegment.BackgroundColor = static_cast<EHealthBarColor>(ReservedBackgroundColor_Black);
    LastSegment.FillPercentage = 1.f;
    HealthSegments.Add(LastSegment);
}

void UBossWidget::UpdateHealthBar(float HealthFraction)
{
    // HealthFraction은 전체 보스 체력의 비율 (0~1)
    int32 NumSegments = HealthSegments.Num();
    if (NumSegments == 0)
    {
        return;
    }

    // 각 세그먼트가 전체 체력의 1/NumSegments를 나타냅니다.
    float SegmentValue = 1.f / NumSegments;
    for (int32 i = 0; i < NumSegments; ++i)
    {
        float LowerBound = i * SegmentValue;
        float UpperBound = (i + 1) * SegmentValue;

        if (HealthFraction >= UpperBound)
        {
            HealthSegments[i].FillPercentage = 1.f;
        }
        else if (HealthFraction <= LowerBound)
        {
            HealthSegments[i].FillPercentage = 0.f;
        }
        else
        {
            HealthSegments[i].FillPercentage = (HealthFraction - LowerBound) / SegmentValue;
        }
    }
}
