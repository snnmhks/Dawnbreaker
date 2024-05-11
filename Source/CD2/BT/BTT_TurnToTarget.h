// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TurnToTarget.generated.h"

/**
 * Ư�� ���ͳ� ��ġ �������� ĳ���͸� ȸ����Ű�� BTT
 */
UCLASS()
class CD2_API UBTT_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// Ư�� ����
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActor;

	// Ư�� ��ġ
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocation;

	UBTT_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
