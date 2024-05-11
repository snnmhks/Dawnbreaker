// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/EnemyAIController.h"
#include "Character/Enemy/EnemyCharacterBase.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EQS/EQC_GetTargetPlayer.h"

void UEQC_GetTargetPlayer::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AEnemyAIController* EAIC = Cast<AEnemyCharacterBase>(QueryInstance.Owner.Get())->GetEnemyAIController();
	AActor* Target = Cast<AActor>(EAIC->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::KeyTarget));

	if (Target)
	{
		UEnvQueryItemType_Point::SetContextHelper(ContextData, Target->GetActorLocation());
	}
}
