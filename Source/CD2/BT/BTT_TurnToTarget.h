// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TurnToTarget.generated.h"

/**
 * 특정 엑터나 위치 방향으로 캐릭터를 회전시키는 BTT
 */
UCLASS()
class CD2_API UBTT_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// 특정 엑터
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActor;

	// 특정 위치
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocation;

	UBTT_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
