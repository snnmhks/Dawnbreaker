// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_InitialMode.generated.h"

/**
 * �ʱ� ���������� �����ϴ� BTT
 * ������� ���̻� ���� ã�� ���ϸ� ���ó�� �ɾ�ٴѴٴ��� �� �� ���� BTT
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
