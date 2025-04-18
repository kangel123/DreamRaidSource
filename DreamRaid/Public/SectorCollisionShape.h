#pragma once

#include "CoreMinimal.h"
#include "BaseCollisionShape.h"
#include "SectorCollisionShape.generated.h"

UCLASS(Blueprintable)
class DREAMRAID_API USectorCollisionShape : public UBaseCollisionShape
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Collision")
    void SetShape(float InRadius, float InAngleDegree);

    virtual bool IsInside(const FVector& Point, const FVector& Origin, const FRotator& Rotation) const override;

private:
    UPROPERTY(EditAnywhere, Category="Collision")
    float Radius = 500.f;

    UPROPERTY(EditAnywhere, Category="Collision")
    float AngleDegree = 90.f;
};
