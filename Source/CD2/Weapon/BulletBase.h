// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:
	ABulletBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float BulletSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
	float BulletLifeTime;

	void FireBullet(FVector Direction);

	UFUNCTION()
	void BulletDestroy(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void BulletDsetroyTimer();

	UFUNCTION(BlueprintNativeEvent)
	void SpawnBulletEffcet(FVector ImpacNormal);
	virtual void SpawnBulletEffcet_Implementation(FVector ImpacNormal);

	FTimerHandle BulletDestroyHandle;

protected:
	virtual void BeginPlay() override;
};
