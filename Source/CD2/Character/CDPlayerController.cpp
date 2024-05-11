// Fill out your copyright notice in the Description page of Project Settings.


#include "CDPlayerController.h"
#include "CD2/GameAbilitySystems/CDAbilitySystemComponent.h"
#include "CDCharacterState.h"

void ACDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACDCharacterState* PS = GetPlayerState<ACDCharacterState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

