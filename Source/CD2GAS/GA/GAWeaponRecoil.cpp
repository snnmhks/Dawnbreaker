// Fill out your copyright notice in the Description page of Project Settings.

#include "CD2/Character/CD2Character/CD2Character.h"
#include "AbilitySystemComponent.h"
#include "Tag/CDGameplayTag.h"
#include "GA/GAWeaponRecoil.h"

UGAWeaponRecoil::UGAWeaponRecoil()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGAWeaponRecoil::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGAWeaponRecoil::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	/*
	ACD2Character* PC = CastChecked<ACD2Character>(ActorInfo->AvatarActor.Get());
	if (PC)
	{
		float RV = PC->MaxRecoil * (1 - FMath::Exp(PC->RecoilScale * (PC->MaxBullet - PC->RemainBullet)));
		PC->SetRecoilValue(RV);
	}
	*/
}
