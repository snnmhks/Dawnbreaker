// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "BT/BTT_ClearAllBBValue.h"

UBTT_ClearAllBBValue::UBTT_ClearAllBBValue()
{
	NodeName = TEXT("ClearBBValue");
}

EBTNodeResult::Type UBTT_ClearAllBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();

	for (const auto BBK : BBKeys)
	{
		BBComponent->ClearValue(BBK.SelectedKeyName);
	}

	return EBTNodeResult::Failed;
}
