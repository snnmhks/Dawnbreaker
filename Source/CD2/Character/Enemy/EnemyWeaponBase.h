// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWeaponBase.generated.h"

UCLASS(BlueprintType)
class CD2_API AEnemyWeaponBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WeaponStaticMesh;

	UPROPERTY(VisibleAnywhere)
	class UWeaponAnimInstance* WeaponAnimInstance;

public:	
	// Sets default values for this actor's properties
	AEnemyWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	bool bIsBulletInfinite = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	float MaxBullet;
	UPROPERTY()
	float RemainBullet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow")
	class UArrowComponent* WeaponFirePoint;

	FVector Target = FVector::ZeroVector;

	// 산탄 정도를 나타냄 값이 높을수록 산탄 정도가 심해짐
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	float SpreadScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class ABulletBase> BulletClass = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool IsPossibleWeaponFire();
	UFUNCTION()
	void WeaponFire();
	UFUNCTION()
	void ShootBullet(FVector Direction);
};
