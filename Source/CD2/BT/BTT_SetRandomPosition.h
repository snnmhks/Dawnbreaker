// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetRandomPosition.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API UBTT_SetRandomPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetRandomPosition();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
