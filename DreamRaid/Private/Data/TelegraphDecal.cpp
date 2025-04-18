#include "TelegraphDecal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"

UTelegraphDecal::UTelegraphDecal()
{
    TelegraphType = ETelegraphType::Decal;
    DecalMaterial = nullptr;
}

void UTelegraphDecal::StartTelegraph_Implementation(FVector Location, FRotator Rotation)
{
    UWorld* World = GetWorld();
    if (World && DecalMaterial)
    {
        FVector DecalSize(200.f, 200.f, 200.f);
        UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAtLocation(World, DecalMaterial, DecalSize, Location, Rotation, 10.0f);
        DecalComponent = DecalComp;
    }
}

void UTelegraphDecal::StopTelegraph_Implementation()
{
    if (DecalComponent.IsValid())
    {
        DecalComponent->DestroyComponent();
        DecalComponent = nullptr;
    }
}
