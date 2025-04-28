#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectAssetBase.h"
#include "CustomTriggerBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCustomTrigger, Log, All);

UENUM(BlueprintType)
enum class ETriggerShapeType : uint8
{
    Box,
    Sphere,
    Capsule,
    Custom
};

class UShapeComponent;
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UBaseCollisionShape;

UCLASS(Abstract)
class DREAMRAID_API ACustomTriggerBase : public AActor
{
    GENERATED_BODY()

public:
    ACustomTriggerBase();

    // 모양
    UPROPERTY(EditAnywhere, Category="Trigger|Shape")
    ETriggerShapeType ShapeType = ETriggerShapeType::Box;

    // 박스
    UPROPERTY(EditAnywhere, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Box"))
    FVector BoxExtent = FVector(150.f);

    // 원
    UPROPERTY(EditAnywhere, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Sphere"))
    float SphereRadius = 150.f;

    // 캡슐
    UPROPERTY(EditAnywhere, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Capsule"))
    float CapsuleRadius = 60.f;

    UPROPERTY(EditAnywhere, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Capsule"))
    float CapsuleHalfHeight = 120.f;

    // 커스텀
    UPROPERTY(EditAnywhere, Category="Trigger|Shape", meta=(EditCondition="ShapeType==ETriggerShapeType::Custom"))
    TObjectPtr<UBaseCollisionShape> CustomCollisionShape;

    // 검사할 액터 종류(예, Character클래스)
    UPROPERTY(EditAnywhere, Category="Trigger|Target")
    TSubclassOf<AActor> TargetActorClass;

    // 액터에 태그가 있는 경우만(예, Player)
    UPROPERTY(EditAnywhere, Category="Trigger|Target")
    FName RequiredTargetTag = NAME_None;
    
    // FX 데이터 에셋들
    UPROPERTY(EditAnywhere, Instanced, Category="Trigger|FX")
    UEffectAssetBase* FX_OnActorEnter;
    
    UPROPERTY(EditAnywhere, Instanced, Category="Trigger|FX")
    UEffectAssetBase* FX_OnActorExit;
    
    UPROPERTY(EditAnywhere, Instanced, Category="Trigger|FX")
    UEffectAssetBase* FX_OnAnyEnter;
    
    UPROPERTY(EditAnywhere, Instanced, Category="Trigger|FX")
    UEffectAssetBase* FX_OnNoneInside;

    
protected:
    // 런타임 생성 콜리전 컴포넌트
    UPROPERTY()
    TObjectPtr<UShapeComponent> CollisionComp = nullptr;

    // 대상 목록 및 입장 상태 맵
    UPROPERTY(Transient)
    TArray<TObjectPtr<AActor>> TargetActors;
    TMap<TObjectPtr<AActor>, bool> WasInsideMap;
    
    bool bWasAnyInside = false;
    
    
    UPROPERTY(Transient)
    TMap<UEffectAssetBase*, UObject*> ActiveEffectHandles;
protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay()  override;
  
    // overlap 이벤트 핸들러
    UFUNCTION()
    void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp,
                               AActor* OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex,
                               bool bFromSweep,
                               const FHitResult& SweepResult);

    UFUNCTION()
    void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp,
                             AActor* OtherActor,
                             UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex);
    
    virtual void EvaluateTrigger(); // 트리거 실행

    void RunFX(UEffectAssetBase* FXData, FVector Location, FRotator Rotation);
    void StopFX(UEffectAssetBase* FXData, UObject* EffectHandle = nullptr);
    
    void RefreshTargetActors(); // 타겟 액터 확인 클래스
    void RebuildCollisionComponent();   // 리빌드 콜리전 컴포넌트
    
    bool AreAllTargetsInside() const;   // 모든
    bool IsAnyoneInside() const;    // 하나라도
    bool IsActorInside(AActor* Actor) const;    // 해당 액터
    
};
