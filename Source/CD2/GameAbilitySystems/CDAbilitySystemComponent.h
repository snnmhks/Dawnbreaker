// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CDAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FReceiveDamageDelegate, UCDAbilitySystemComponent* SourceACS, float UnmitigatedDamage, float MitigatedDamage);

/**
 * 
 */
UCLASS()
class CD2_API UCDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceiveDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UCDAbilitySystemComponent* SourceACS, float UnmitigatedDamage, float MitigatedDamage);
};
