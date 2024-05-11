// Fill out your copyright notice in the Description page of Project Settings.

#include "CD2GAS/Tag/CDGameplayTag.h"
#include "CDGameplayAbility.h"
#include "CDAbilitySystemComponent.h"

UCDGameplayAbility::UCDGameplayAbility() 
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(CDTAG_Character_Test);
	ActivationOwnedTags.AddTag(CDTAG_Character_Tests);
	//ActivationBlockedTags.AddTag(CDTAG_Character_State_Dead);
	//ActivationBlockedTags.AddTag(CDTAG_Character_Debuf_Stun);
}

void UCDGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UCDGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AActor* Abataractor = ActorInfo->AvatarActor.Get();
	if (Abataractor)
	{
		UE_LOG(LogTemp, Log, TEXT("Move Ability Cancel"));
	}
}

void UCDGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Abataractor = ActorInfo->AvatarActor.Get();
	if (Abataractor)
	{
		UE_LOG(LogTemp, Log, TEXT("Move Ability Activate"));
	}
}
