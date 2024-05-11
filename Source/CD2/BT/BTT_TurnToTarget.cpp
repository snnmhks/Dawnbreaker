// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/EnemyCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BT/BTT_TurnToTarget.h"

UBTT_TurnToTarget::UBTT_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");

	TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_TurnToTarget, TargetActor), AActor::StaticClass());
	TargetLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_TurnToTarget, TargetLocation));
}

EBTNodeResult::Type UBTT_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacterBase* ControllingPawn = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();

	if (nullptr == ControllingPawn || BBComponent == nullptr) return EBTNodeResult::Failed;
	else
	{
		AActor* _TargetActor = Cast<AActor>(BBComponent->GetValueAsObject(TargetActor.SelectedKeyName));
		FVector _TargetLocation = BBComponent->GetValueAsVector(TargetLocation.SelectedKeyName);

		if (_TargetActor == nullptr && _TargetLocation == FVector::ZeroVector) return EBTNodeResult::Failed;

		// 타겟 방향 벡터 확보
		FVector TargetVector = _TargetLocation;
		if (_TargetActor != nullptr) TargetVector = _TargetActor->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), TargetVector);

		// AIM offset값 설정 및 회전
		ControllingPawn->AimPitchValue = TargetRotation.Pitch;
		TargetRotation.Pitch = 0.0f;
		ControllingPawn->SetActorRotation(TargetRotation);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
