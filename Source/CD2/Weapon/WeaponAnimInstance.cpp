// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "WeaponAnimInstance.h"
#include "AnimNotify/AnimNotify_PlaySoundAndReport.h"

UWeaponAnimInstance::UWeaponAnimInstance() {

}

void UWeaponAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Weapon = Cast<AWeapon>(TryGetPawnOwner());
}

float UWeaponAnimInstance::PlayMongtage(FName Name, float PlaySpeed) {
	if (Name == "Fire" && IsValid(FireMontage)) {
		return Montage_Play(FireMontage, PlaySpeed);
	}
	else if (Name == "Reload" && IsValid(ReloadMontage))
	{
		return Montage_Play(ReloadMontage, PlaySpeed);
	}
	else return 0.0f;
}

void UWeaponAnimInstance::AnimNotify_ReloadEnd()
{
	OnReloadEndCheck.Broadcast();
}

bool UWeaponAnimInstance::IsPlayFireMontage()
{
	return Montage_IsPlaying(FireMontage);
}

bool UWeaponAnimInstance::IsPlayReloadMontage()
{
	return Montage_IsPlaying(ReloadMontage);
}
