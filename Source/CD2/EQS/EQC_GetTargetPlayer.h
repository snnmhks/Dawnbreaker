// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_GetTargetPlayer.generated.h"

/**
 * 포착한 적을 받아오는 컨텍스트
 */

struct FEnvQueryContextData;
struct FEnvQueryInstance;
class AEnemyCharacterBase;
class AEnemyAIController;
class UBlackboardComponent;
class AActor;

UCLASS()
class CD2_API UEQC_GetTargetPlayer : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
