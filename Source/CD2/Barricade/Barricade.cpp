// Fill out your copyright notice in the Description page of Project Settings.

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Barricade/Barricade.h"

ABarricade::ABarricade()
{

}

void ABarricade::BarriacadeDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
    AGeometryCollectionActor* HittedBarricade = Cast<AGeometryCollectionActor>(DamagedActor);

    if (HittedBarricade)
    {
        if (HittedBarricade->GetGeometryCollectionComponent()->DamageThreshold[0] > 0)
        {
            HittedBarricade->GetGeometryCollectionComponent()->DamageThreshold[0] -= Damage;
        }
        else
        {
            HittedBarricade->GetGeometryCollectionComponent()->SetEnableDamageFromCollision(true);
            HittedBarricade->GetGeometryCollectionComponent()->CrumbleActiveClusters();
        }
    }
}

void ABarricade::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> GetActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeometryCollectionActor::StaticClass(), GetActors);

    for (AActor* Actors : GetActors)
    {
        GeometryActors.Add(Cast<AGeometryCollectionActor>(Actors));
    }

    for (AGeometryCollectionActor* GAs : GeometryActors)
    {
        GAs->OnTakePointDamage.AddDynamic(this, &ABarricade::BarriacadeDamage);
        GAs->GetGeometryCollectionComponent()->SetNotifyRigidBodyCollision(true);
        GAs->GetGeometryCollectionComponent()->SetSimulatePhysics(true);
        GAs->GetGeometryCollectionComponent()->SetEnableDamageFromCollision(false);
    }
}

void ABarricade::BreakBarricade(float Damage, FVector HittedLocation, FVector HittedDirection)
{
    
}
