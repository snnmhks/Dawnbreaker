// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Character/Enemy/EnemyAIController.h"
#include "BT/BTT_SetWalkSpeed.h"

UBTT_SetWalkSpeed::UBTT_SetWalkSpeed()
{
	NodeName = TEXT("SetWalkSpeed");
}

EBTNodeResult::Type UBTT_SetWalkSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* ControllingPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn) return EBTNodeResult::Failed;

	ControllingPawn->GetCharacterMovement()->MaxWalkSpeed = Speed;

	return EBTNodeResult::Succeeded;
}
