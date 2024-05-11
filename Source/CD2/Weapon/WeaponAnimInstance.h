// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponNotifyCheckDelegate);

/**
 * 
 */
UCLASS()
class CD2_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* Weapon = nullptr;

	// Weapon Fire Montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Action)
	class UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Action)
	class UAnimMontage* ReloadMontage;

	FOnWeaponNotifyCheckDelegate OnReloadEndCheck;

public:
	UWeaponAnimInstance();

	UFUNCTION()
	float PlayMongtage(FName Name, float PlaySpeed);

	UFUNCTION()
	void AnimNotify_ReloadEnd();

	UFUNCTION()
	bool IsPlayFireMontage();

	UFUNCTION()
	bool IsPlayReloadMontage();

private:

protected:
	virtual void NativeBeginPlay() override;
};
