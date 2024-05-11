// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyCharacterBase.h"
#include "Character/Enemy/EnemyAIController.h"
#include "BT/BTT_InitialMode.h"

UBTT_InitialMode::UBTT_InitialMode()
{
	NodeName = TEXT("InitialMode");
}

EBTNodeResult::Type UBTT_InitialMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacterBase* ControllingPawn = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn) return EBTNodeResult::Failed;

	ControllingPawn->WalkMode();
	ControllingPawn->AimPitchValue = 0.0f;

	return EBTNodeResult::Succeeded;
}
