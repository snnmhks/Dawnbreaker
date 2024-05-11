// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearAllBBValue.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API UBTT_ClearAllBBValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	TArray<FBlackboardKeySelector> BBKeys;

	UBTT_ClearAllBBValue();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
