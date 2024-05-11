// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyCharacterBase.h"
#include "Character/Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BT/BTT_GetTargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"

UBTT_GetTargetPoint::UBTT_GetTargetPoint()
{
	NodeName = TEXT("GetTargetPoint");
}

EBTNodeResult::Type UBTT_GetTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacterBase* ControllingPawn = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

	TArray<AActor*> TargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetPoints);

	if (TargetPoints.Num() > 0)
	{
		ATargetPoint* SelectedTargetPoint = Cast<ATargetPoint>(TargetPoints[0]); 
		ControllingPawn->WalkMode();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::KeyTargetPointLocation, SelectedTargetPoint->GetActorLocation());
	}

	return EBTNodeResult::Type();
}
