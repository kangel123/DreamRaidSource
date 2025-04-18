#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaSkillActor.generated.h"

class USphereComponent;


UCLASS()
class DREAMRAID_API AAreaSkillActor : public AActor
{
    GENERATED_BODY()
    
public:
    AAreaSkillActor();

    
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Area Skill")
    USphereComponent* CollisionComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Skill")
    float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Skill")
    float DamageTickInterval;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Area Skill")
    bool bCollisionActive;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Area Skill")
    TArray<AActor*> OverlappingActors;

    // 충돌 영역을 활성화하면서 데미지 타이머를 시작합니다.
    UFUNCTION(BlueprintCallable, Category = "Area Skill")
    void ActivateCollision(float CollisionRadius);

    // 충돌 영역을 비활성화하고 데미지 타이머를 중단합니다.
    UFUNCTION(BlueprintCallable, Category = "Area Skill")
    void DeactivateCollision();

protected:
    virtual void BeginPlay() override;

    FTimerHandle DamageTimerHandle;

    UFUNCTION()
    void ApplyDamageTick();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
