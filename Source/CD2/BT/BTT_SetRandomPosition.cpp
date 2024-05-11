// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyCharacterBase.h"
#include "Character/Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BT/BTT_SetRandomPosition.h"

UBTT_SetRandomPosition::UBTT_SetRandomPosition()
{
	NodeName = TEXT("SetRandomPosition");
}

EBTNodeResult::Type UBTT_SetRandomPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacterBase* ControllingPawn = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	FVector Orgin = ControllingPawn->GetActorLocation();
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Orgin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::KeyRandomPosition, NextPatrol.Location);
		ControllingPawn->WalkMode();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}
