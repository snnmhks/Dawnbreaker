// Fill out your copyright notice in the Description page of Project Settings.

#include "CDGASMove.h"
#include "AbilitySystemComponent.h"
#include "Tag/CDGameplayTag.h"

UCDGASMove::UCDGASMove()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(CDTAG_Character_Test);
}

void UCDGASMove::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UCDGASMove::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AActor* Abataractor = ActorInfo->AvatarActor.Get();
	if (Abataractor)
	{
		UE_LOG(LogTemp, Log, TEXT("Move Ability Cancel"));
	}
}

void UCDGASMove::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Abataractor = ActorInfo->AvatarActor.Get();
	if (Abataractor)
	{
		UE_LOG(LogTemp, Log, TEXT("Move Ability Activate"));
	}
}
