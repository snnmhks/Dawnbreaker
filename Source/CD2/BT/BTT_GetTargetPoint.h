// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API UBTT_GetTargetPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_GetTargetPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
