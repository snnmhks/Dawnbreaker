// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GameFramework/Actor.h"
#include "Barricade.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API ABarricade : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	float CommulativeDamage;

public:
	ABarricade();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry Actors")
	TArray<class AGeometryCollectionActor*> GeometryActors;

protected:
	UFUNCTION()
	void BarriacadeDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	virtual void BeginPlay() override;
	virtual void BreakBarricade(float Damage, FVector HittedLocation, FVector HittedDirection);
};
