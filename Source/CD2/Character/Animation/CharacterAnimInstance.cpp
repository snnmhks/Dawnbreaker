// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"
#include "Character/CD2Character/CD2Character.h"
#include "AnimNotify/AnimNotify_PlaySoundAndReport.h"
#include "GameFramework/CharacterMovementComponent.h"

TMap<UAnimMontage*, EStateEnum> UCharacterAnimInstance::CustomTagMap;

UCharacterAnimInstance::UCharacterAnimInstance() 
{

}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayMontage("Equip", 1.0f);
}

void UCharacterAnimInstance::NativeUpdateAnimation(float Delta)
{
	Super::NativeUpdateAnimation(Delta);

	if (Character)
	{
		CalculateSpeed();
		CalculateState();
		CalculatePitchValue();
	}
}

void UCharacterAnimInstance::AddCustomTag(UAnimMontage* _Montage, EStateEnum _MontageTag)
{
	if (CustomTagMap.Contains(_Montage)) return;

	CustomTagMap.Add(_Montage, _MontageTag);
}

void UCharacterAnimInstance::DeleteCustomTag(UAnimMontage* _Montage)
{
	if (CustomTagMap.Contains(_Montage)) CustomTagMap.FindAndRemoveChecked(_Montage);
}

EStateEnum UCharacterAnimInstance::GetMontageTag(UAnimMontage* _Montage)
{
	if (CustomTagMap.Contains(_Montage)) return CustomTagMap.FindChecked(_Montage);

	return EStateEnum::STATE_None;
}

void UCharacterAnimInstance::AnimNotify_CallWeaponFire()
{
	OnWeaponFireCheck.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_EquipWeapon()
{
	OnWeaponEquipCheck.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_EquipEnd()
{
	bIsEquip = true;
}

void UCharacterAnimInstance::AnimNotify_ReloadEnd()
{
	OnWeaponReloadEndCheck.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_AttackStart()
{
	AddCustomTag(FireMontage, EStateEnum::STATE_Attack);
	OnAttackStart.Broadcast();
}

void UCharacterAnimInstance::CalculateSpeed()
{
	FVector Character2DSpeed = CharacterMC->Velocity;
	ZSpeed = Character2DSpeed.Z;
	Character2DSpeed.Z = 0;
	Speed2D.X = FVector::DotProduct(Character->GetActorRightVector(), Character2DSpeed);
	Speed2D.Y = FVector::DotProduct(Character->GetActorForwardVector(), Character2DSpeed);

	GroundSpeed = Character2DSpeed.Length();

	bShoudMove = (GroundSpeed > 2.0f);
}

void UCharacterAnimInstance::CalculateState()
{
	bIsAiming = Character->IsAiming;
	bIsCrouching = Character->bIsCrouching;
	bIsSliding = Character->bIsSliding;
	bIsFalling = CharacterMC->IsFalling();
}

void UCharacterAnimInstance::CalculatePitchValue()
{
	if (!bIsAiming)
	{
		AimOffsetPitchValue = 0;
		return;
	}

	AimOffsetPitchValue = Character->CollectionPitchValue;
}

float UCharacterAnimInstance::PlayMontage(FName Name, float PlaySpeed)
{
	if (Name == "Fire") return Montage_Play(FireMontage, PlaySpeed);
	else if (Name == "Reload") return Montage_Play(ReloadMontage, PlaySpeed);
	else if (Name == "Equip") return Montage_Play(WeaponEquipMontage, PlaySpeed);

	return 0.0f;
}
