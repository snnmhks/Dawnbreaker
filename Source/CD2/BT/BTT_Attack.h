// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class CD2_API UBTT_AttackPossible : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_AttackPossible();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
