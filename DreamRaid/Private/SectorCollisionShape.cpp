#include "SectorCollisionShape.h"

void USectorCollisionShape::SetShape(float InRadius, float InAngleDegree)
{
    Radius = InRadius;
    AngleDegree = InAngleDegree;
}

bool USectorCollisionShape::IsInside(const FVector& Point, const FVector& Origin, const FRotator& Rotation) const
{
    FVector ToTarget = Point - Origin;
    float Dist = ToTarget.Size();
    if (Dist > Radius) return false;

    FVector Forward = Rotation.Vector();
    FVector Dir = ToTarget.GetSafeNormal();
    float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(Forward, Dir)));

    return Angle <= AngleDegree * 0.5f;
}
