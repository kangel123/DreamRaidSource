#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebuffTypes.h"

#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "Arrow.generated.h"

// 화살의 종류
UENUM(BlueprintType)
enum class EArrowType : uint8
{
    Normal      UMETA(DisplayName = "Normal"),
    Penetrating UMETA(DisplayName = "Penetrating"),
    Area        UMETA(DisplayName = "Area")
};

UCLASS()
class DREAMRAID_API AArrow : public AActor
{
    GENERATED_BODY()
    
public:
    AArrow();

    // ArrowMesh 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ArrowMesh;

    // ProjectileMovement 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovement;

    // 화살 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
    EArrowType ArrowType;

    // 화살 디버프 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
    EDebuffType DebuffType;

    // 최종 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
    float FinalDamageValue;

    // 화살 발사 여부
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow")
    bool IsFire;

    // 충돌 가능한 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
    TArray<FName> ValidHitTags;

    // 화살 발사 FX (인덱스: 0-Normal, 1-Fire, 2-Ice, 3-Darkness)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    TArray<UNiagaraSystem*> ArrowFlightFXArray;

    // 화살 발사 사운드
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    USoundCue* FireSoundCue;

    // 적중 FX (인덱스: 0-Normal, 1-Fire, 2-Ice, 3-Darkness)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    TArray<UNiagaraSystem*> ArrowHitFXArray;

    // 적중 사운드
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    USoundCue* HitSoundCue;

    // 텔레그래프
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    class UNiagaraComponent* TelegraphFXComponent;
    
    // 화살 발사 함수
    UFUNCTION(BlueprintCallable, Category = "Arrow")
    void FireArrow(const FVector& ShootDirection, float Speed, float InDamage);
    
protected:
    virtual void BeginPlay() override;

    // Penetrating 타입에서 중복 판정을 막기 위한 변수
    TSet<AActor*> AlreadyHitActors;

    // Hit 판정 함수
    UFUNCTION()
    void OnArrowHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // Overlap 판정 함수 (Penetrating 타입)
    UFUNCTION()
    void OnArrowOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 속성 효과 적용 함수
    void ApplyArrowAttributeEffect(AActor* Target, const FVector& ImpactPoint);

    // 개별 속성 효과 함수들
    void ApplyFireDebuff(AActor* Target, const FVector& ImpactPoint);
    void ApplyIceDebuff(AActor* Target, const FVector& ImpactPoint);
    void ApplyDebuffMaterialEffect(AActor* Target, const FVector& ImpactPoint);
};
