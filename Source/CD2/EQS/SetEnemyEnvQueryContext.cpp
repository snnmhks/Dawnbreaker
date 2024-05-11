// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyAIController.h"
#include "Character/Enemy/EnemyCharacterBase.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EQS/SetEnemyEnvQueryContext.h"

void USetEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    AEnemyAIController* EAIC = Cast<AEnemyCharacterBase>(QueryInstance.Owner.Get())->GetEnemyAIController();
    const AActor* DamageCauser = EAIC->DamageCauser;

    if (DamageCauser) UEnvQueryItemType_Point::SetContextHelper(ContextData, DamageCauser->GetActorLocation());
}
