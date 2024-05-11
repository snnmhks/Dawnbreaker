// Fill out your copyright notice in the Description page of Project Settings.


#include "CDAbilitySystemComponent.h"

void UCDAbilitySystemComponent::ReceiveDamage(UCDAbilitySystemComponent* SourceACS, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceACS, UnmitigatedDamage, MitigatedDamage);
}
