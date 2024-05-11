// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "EnvironmentQuery/EnvQueryManager.h"

const FName AEnemyAIController::KeyTarget(TEXT("Target"));
const FName AEnemyAIController::KeyDetectedLocation(TEXT("DetectedLocation"));
const FName AEnemyAIController::KeyRandomPosition(TEXT("RandomPosition"));
const FName AEnemyAIController::KeyPursuitPosition(TEXT("PursuitPosition"));
const FName AEnemyAIController::KeyTargetPointLocation(TEXT("TargetPointLocation"));
const FName AEnemyAIController::KeyHideLocation(TEXT("HideLocation"));
const FName AEnemyAIController::KeyIsPossibleAttack(TEXT("IsPossibleAttack"));
const FName AEnemyAIController::KeyHearingLocation(TEXT("HearingLocation"));

//const FName AEnemyAIController::KeyShootedLocation = FName(TEXT("ShootedLocation"));

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BLACK_BOARD(TEXT("BlackboardData'/Game/BehaviorTree/BB_Enemy.BB_Enemy'"));
	if (BLACK_BOARD.Succeeded()) BBAsset = BLACK_BOARD.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BEHAVIOR_TREE(TEXT("BehaviorTree'/Game/BehaviorTree/BT_Enemy.BT_Enemy'"));
	if (BLACK_BOARD.Succeeded()) BTAsset = BEHAVIOR_TREE.Object;

	EnemyPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerception"));
}

void AEnemyAIController::RunBT()
{
	// 정확한 작동 원리를 아직 모름
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp)) RunBehaviorTree(BTAsset);
}

void AEnemyAIController::StopBT()
{
	// 지금 현재 작동중안 BT를 가져와서 있으면 멈춘다.
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BehaviorTreeComponent) return;

	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void AEnemyAIController::RunEQS()
{
	FEnvQueryRequest HidingSpotQueryRequest = FEnvQueryRequest(EnemyEQS, GetPawn());
	HidingSpotQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AEnemyAIController::MoveToQueryResult);
}

void AEnemyAIController::MoveToQueryResult(TSharedPtr<FEnvQueryResult> result)
{
	if (result->IsSuccsessful()) 
	{
		GetBlackboardComponent()->SetValueAsVector(KeyHideLocation, result.Get()->GetItemAsLocation(0));
	}
}

void AEnemyAIController::EnemyDamageReport(UObject* WorldContextObject, AActor* DamagedActor, AActor* _Instigator, float DamageAmount, FVector EventLocation, FVector HitLocation)
{
	UAISense_Damage::ReportDamageEvent(WorldContextObject, DamagedActor, _Instigator, DamageAmount, EventLocation, HitLocation);
}

void AEnemyAIController::EnemyNoiseReport()
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetPawn()->GetActorLocation(), 1.0f, this, 1000.0f);
}

void AEnemyAIController::EnemyPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (UpdatedActors.IsEmpty()) return;

	for (AActor* DetectedActor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		EnemyPerception->GetActorsPerception(DetectedActor, Info);

		for (const FAIStimulus Stimuls : Info.LastSensedStimuli)
		{
			if (Stimuls.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
			{
				if (Stimuls.WasSuccessfullySensed())
				{
					DetectPlayer(DetectedActor);
				}
				else
				{
					NoneDetectPlayer();
					GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::KeyPursuitPosition, Stimuls.StimulusLocation);
				}
			}
			else if (Stimuls.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
			{
				if (Stimuls.WasSuccessfullySensed())
				{
					DamageCauser = DetectedActor;
					GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::KeyDetectedLocation, Stimuls.StimulusLocation);
					RunEQS();
				}
			}
			else if (Stimuls.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
			{
				if (Stimuls.WasSuccessfullySensed())
				{
					GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::KeyHearingLocation, Stimuls.StimulusLocation);
				}
			}
		}
	}
}

void AEnemyAIController::DisableSightPerception()
{
	EnemyPerception->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
	NoneDetectPlayer();
}

void AEnemyAIController::SetBBValueAsBool(FName _Key, bool _Value)
{
	GetBlackboardComponent()->SetValueAsBool(_Key, _Value);
}

void AEnemyAIController::ClearBBValue(FName _Key)
{
	GetBlackboardComponent()->ClearValue(_Key);
}

void AEnemyAIController::RunMode()
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 500;
}

void AEnemyAIController::WalkMode()
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 200;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBT();
	EnemyPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::EnemyPerceptionUpdated);
}

void AEnemyAIController::NoneDetectPlayer()
{
	if (GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::KeyTarget))
	{
		
	}
}

void AEnemyAIController::DetectPlayer(AActor* DetectedPlayer)
{
	GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::KeyTarget, DetectedPlayer);
	if (GetBlackboardComponent()->GetValueAsVector(AEnemyAIController::KeyDetectedLocation) != FVector::ZeroVector)
	{
		
	}
}
