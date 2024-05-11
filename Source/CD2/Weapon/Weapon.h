// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UCharacterAnimInstance;

UCLASS(BlueprintType)
class CD2_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	class UWeaponAnimInstance* WeaponAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterAnim")
	TSubclassOf<UCharacterAnimInstance> LinkedCharacterAnimClass = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float MaxVerticalRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float MaxHorizontalRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float RecoilScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	int32 RecoilStartBulletNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	bool bIsBulletInfinite = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	float MaxBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	float BulletRange;
	UPROPERTY()
	float RemainBullet;

	UPROPERTY()
	bool bIsAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RPM")
	float FireSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	float PitchCollection = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow")
	class UArrowComponent* WeaponFirePoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Decal")
	class UMaterialInterface* WeaponEmitterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrawBulletLine")
	float DrawLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrawBulletLine")
	uint8 DrawDepthPriority;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrawBulletLine")
	float LineThickness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Emitter")
	class UParticleSystem* FireEmitter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Emitter")
	class USoundBase* HeadHitSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
	class UAIPerceptionStimuliSourceComponent* WeaponPerceptionSource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class ABulletBase> BulletClass = nullptr;

public:	
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION()
	class UClass* GetLinkedCharacterAnimClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnDecalAtLocation(class UMaterialInterface* DecalMaterial, FVector SpawnLocation);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BluePrintCallable)
	USkeletalMeshComponent* GetMesh();

	UFUNCTION()
	FVector GetWeaponForwardVector();
	UFUNCTION()
	FVector GetWeaponLocation();
	UFUNCTION()
	void WeaponReload();
	UFUNCTION()
	void WeaponFire(FVector DirectionCorrection);
	UFUNCTION()
	void FireBullet(FVector DirectionCorrection);
	UFUNCTION()
	void DrawBulletLine(FVector DirectionCorrection);
	UFUNCTION()
	void SpawnFireEmitter();
	UFUNCTION()
	void ShootBullet(FVector Direction);

	UFUNCTION()
	bool IsFire();
	UFUNCTION()
	bool IsReload();

	UFUNCTION(BluePrintCallable)
	float PlayFireMontage();
	UFUNCTION(BluePrintCallable)
	float PlayReloadMontage();
};
