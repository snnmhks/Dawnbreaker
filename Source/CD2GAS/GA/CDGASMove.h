// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../CD2/CD2.h"
#include "Abilities/GameplayAbility.h"
#include "CDGASMove.generated.h"

/**
 * 
 */
UCLASS()
class CD2GAS_API UCDGASMove : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCDGASMove();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	CDAbilityID AbilityInputID = CDAbilityID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	CDAbilityID AbilityID = CDAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	bool ActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
