// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

UCLASS()
class CD2_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	// Behavior Tree를 담을 변수
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	// Black Board를 담을 변수
	UPROPERTY()
	class UBlackboardData* BBAsset;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
	class UAIPerceptionComponent* EnemyPerception;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UEnvQuery* EnemyEQS;

	UPROPERTY()
	class AActor* DamageCauser = nullptr;

	static const FName KeyTarget;
	static const FName KeyDetectedLocation;
	static const FName KeyRandomPosition;
	static const FName KeyPursuitPosition;
	static const FName KeyTargetPointLocation;
	static const FName KeyHideLocation;
	static const FName KeyIsPossibleAttack;
	static const FName KeyHearingLocation;

	//static const FName KeyShootedLocation;

public:
	AEnemyAIController();
	// BT를 시작하는 함수와 멈추는 함수
	void RunBT();
	void StopBT();

	UFUNCTION()
	void RunEQS();

	// TSharedPtr이 있으면 UFUNCTION이 안됨
	void MoveToQueryResult(TSharedPtr<FEnvQueryResult> result);

	UFUNCTION()
	void EnemyDamageReport(UObject* WorldContextObject, AActor* DamagedActor, AActor* _Instigator, float DamageAmount, FVector EventLocation, FVector HitLocation);

	UFUNCTION()
	void EnemyNoiseReport();

	UFUNCTION()
	void EnemyPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void DisableSightPerception();

	UFUNCTION()
	void SetBBValueAsBool(FName _Key, bool _Value);

	UFUNCTION()
	void ClearBBValue(FName _Key);

	// 뛰냐 걷냐
	void RunMode();
	void WalkMode();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void NoneDetectPlayer();
	void DetectPlayer(AActor* DetectedPlayer);
};
