#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "FootCollision.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREAMRAID_API UFootCollision : public UCapsuleComponent
{
    GENERATED_BODY()

public:
    UFootCollision();

    // 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foot")
    float DamageAmount = 20.f;

    // 에어본 효과: 발사할 Launch Velocity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foot|Airborne")
    FVector AirborneLaunchVelocity = FVector(0.f, 0.f, 600.f);
    
    // 에어본 효과: 적용할 중력 스케일
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foot|Airborne")
    float AirborneReducedGravity = 0.3f;
    
    // 에어본 효과: 지속 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foot|Airborne")
    float AirborneDuration = 3.f;
    
    // 에어본 상태에서의 AirControl
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foot|Airborne")
    float AirControlWhileAirborne = 0.f;
    
    // 충돌 가능한 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foot")
    TArray<FName> ValidHitTags;
    
    // 콜리전 활성화
    UFUNCTION(BlueprintCallable, Category="Foot|Collision")
    void EnableFootCollision();

    // 콜리전 비활성화
    UFUNCTION(BlueprintCallable, Category="Foot|Collision")
    void DisableFootCollision();

    // 데미지 준 대상 초기화
    UFUNCTION(BlueprintCallable, Category="Foot")
    void ResetHitTargets();
    
    // 맞은 대상 확인
    UFUNCTION(BlueprintCallable, Category="Foot")
    bool HasHitTargets() const
    {
        return AlreadyHitActors.Num() > 0;
    }
    
protected:
    virtual void BeginPlay() override;

    // 이미 데미지를 준 대상들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Foot")
    TSet<AActor*> AlreadyHitActors;

    // Overlap 이벤트 처리 함수
    UFUNCTION()
    void HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult & SweepResult);

    // 대상에게 데미지 적용
    void ApplyDamageToTarget(AActor* Target, float InDamageAmount);

    // 에어본 효과 적용
    void ApplyAirborneEffect(AActor* Target, const FVector& LaunchVelocity, float ReducedGravityScale, float EffectDuration, float AirControlValue);
};
