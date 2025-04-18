#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomTriggerBase.generated.h"

/** 트리거 콜리전 모양 */
UENUM(BlueprintType)
enum class ETriggerShapeType : uint8
{
    Box,
    Sphere,
    Capsule,
    Custom
};

class UShapeComponent;
class UBaseCollisionShape;

UCLASS(Abstract) // 직접 인스턴스 불가
class DREAMRAID_API ACustomTriggerBase : public AActor
{
    GENERATED_BODY()

public:
    ACustomTriggerBase();

protected:
    /** 콜리전 형태 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger|Shape")
    ETriggerShapeType ShapeType = ETriggerShapeType::Box;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Box"))
    FVector BoxExtent = FVector(100.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Sphere"))
    float SphereRadius = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Capsule"))
    float CapsuleRadius = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Capsule"))
    float CapsuleHalfHeight = 100.f;

    /** 커스텀 콜리전을 쓸 때 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Custom"))
    UBaseCollisionShape* CustomCollisionShape;

protected:
    /** 실제 충돌 컴포넌트 */
    UPROPERTY()
    UShapeComponent* CollisionComp = nullptr;

    /** 감지할 액터들 (자식이 자동 검색 or BP에서 설정) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger|Detection")
    TArray<AActor*> TargetActors;

    /** 액터별 Inside 여부 추적 */
    TMap<AActor*, bool> WasInsideMap;

    /** 이전 프레임에 한 명이라도 있었는지 */
    bool bHadPlayersLastFrame = false;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 자식이 이를 override해서 TargetActors 채우거나, 다른 로직 가능 */
    UFUNCTION(BlueprintNativeEvent, Category="Trigger")
    void AutoFindActors();

    virtual void AutoFindActors_Implementation();

    /** 콜리전 컴포넌트 생성 */
    void RebuildCollisionComponent();

    UFUNCTION(BlueprintNativeEvent, Category="Trigger")
    void OnTriggerActivated();

    virtual void OnTriggerActivated_Implementation();

    /** 진입/이탈 시점 */
    void HandleTriggerEnter(AActor* Actor);
    void HandleTriggerExit(AActor* Actor);

    /** HasPlayers, NoPlayers (자식이 필요 없다면 override 안 해도 됨) */
    UFUNCTION(BlueprintNativeEvent, Category="Trigger")
    void UpdateHasPlayersFX(bool bAnyInside);

    virtual void UpdateHasPlayersFX_Implementation(bool bAnyInside);
};
