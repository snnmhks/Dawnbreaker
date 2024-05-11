// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetWalkSpeed.generated.h"

/**
 * ĳ������ �ӵ��� �����ϴ� BTT
 */
UCLASS()
class CD2_API UBTT_SetWalkSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Speed;

	UBTT_SetWalkSpeed();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
