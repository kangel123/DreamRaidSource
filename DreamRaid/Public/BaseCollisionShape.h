#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseCollisionShape.generated.h"

UCLASS(Abstract, Blueprintable)
class DREAMRAID_API UBaseCollisionShape : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Collision")
    virtual bool IsInside(const FVector& Point, const FVector& Origin, const FRotator& Rotation) const PURE_VIRTUAL(UBaseCollisionShape::IsInside, return false;);

    UFUNCTION(BlueprintCallable, Category="Collision")
    virtual bool IsOverlapping(AActor* Target, const FVector& Origin, const FRotator& Rotation) const;

    UFUNCTION(BlueprintCallable, Category="Collision")
    virtual bool IsOverlappingWithSkeletalMesh(
        USkeletalMeshComponent* SkeletalMesh,
        const FVector& Origin,
        const FRotator& Rotation,
        const TArray<FName>& BoneNames) const;

protected:
    bool IsAnySamplePointInside(const FBoxSphereBounds& Bounds, const FVector& Origin, const FRotator& Rotation) const;
};
