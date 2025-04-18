#include "BaseCollisionShape.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

bool UBaseCollisionShape::IsOverlapping(AActor* Target, const FVector& Origin, const FRotator& Rotation) const
{
    if (!IsValid(Target) || !Target->GetRootComponent()) return false;
    const FBoxSphereBounds& Bounds = Target->GetRootComponent()->Bounds;
    return IsAnySamplePointInside(Bounds, Origin, Rotation);
}

bool UBaseCollisionShape::IsOverlappingWithSkeletalMesh(
    USkeletalMeshComponent* SkeletalMesh,
    const FVector& Origin,
    const FRotator& Rotation,
    const TArray<FName>& BoneNames) const
{
    if (!IsValid(SkeletalMesh)) return false;

    for (const FName& Bone : BoneNames)
    {
        FVector BoneLocation = SkeletalMesh->GetBoneLocation(Bone, EBoneSpaces::WorldSpace);
        if (IsInside(BoneLocation, Origin, Rotation)) return true;
    }
    return false;
}

bool UBaseCollisionShape::IsAnySamplePointInside(const FBoxSphereBounds& Bounds, const FVector& Origin, const FRotator& Rotation) const
{
    FVector Center = Bounds.Origin;
    FVector Extent = Bounds.BoxExtent;

    TArray<FVector> Points = {
        Center,
        Center + FVector(+Extent.X, +Extent.Y, 0),
        Center + FVector(-Extent.X, +Extent.Y, 0),
        Center + FVector(+Extent.X, -Extent.Y, 0),
        Center + FVector(-Extent.X, -Extent.Y, 0)
    };

    for (const FVector& Point : Points)
    {
        if (IsInside(Point, Origin, Rotation)) return true;
    }

    return false;
}
