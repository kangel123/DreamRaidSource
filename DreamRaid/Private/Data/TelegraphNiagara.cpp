#include "TelegraphNiagara.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"

UTelegraphNiagara::UTelegraphNiagara()
{
    TelegraphType = ETelegraphType::Niagara;
    NiagaraEffect = nullptr;
}

void UTelegraphNiagara::StartTelegraph_Implementation(FVector Location, FRotator Rotation)
{
    UWorld* World = GetWorld();
    if (World && NiagaraEffect)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, NiagaraEffect, Location, Rotation);
        NiagaraComponent = NiagaraComp;
    }
}

void UTelegraphNiagara::StopTelegraph_Implementation()
{
    if (NiagaraComponent.IsValid())
    {
        NiagaraComponent->Deactivate();
        NiagaraComponent->DestroyComponent();
        NiagaraComponent = nullptr;
    }
}
