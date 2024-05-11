// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_InitialMode.generated.h"

/**
 * 초기 설정값으로 셋팅하는 BTT
 * 예를들어 더이상 적을 찾지 못하면 평소처럼 걸어다닌다던지 할 때 쓰는 BTT
 */
UCLASS()
class CD2_API UBTT_InitialMode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Speed;

	UBTT_InitialMode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
