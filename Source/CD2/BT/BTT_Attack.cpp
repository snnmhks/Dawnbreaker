// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BT/BTT_Attack.h"
#include "Components/SkeletalMeshComponent.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacterBase* ControllingPawn = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	else
	{
		// 공격 대상
		AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::KeyTarget));
		if (Target == nullptr) return EBTNodeResult::Failed;

		// 공격
		ControllingPawn->EnemyAttack(Target);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTT_AttackPossible::UBTT_AttackPossible()
{
	NodeName = TEXT("IsAttackPossible");
}

EBTNodeResult::Type UBTT_AttackPossible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacterBase* ControllingPawn = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	else
	{
		// 공격 대상
		AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::KeyTarget));
		if (Target == nullptr) return EBTNodeResult::Failed;

		// 공격
		ControllingPawn->EnemyReadyToAttack(Target);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
