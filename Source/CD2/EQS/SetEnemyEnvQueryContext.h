// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "SetEnemyEnvQueryContext.generated.h"

/**
 * 공격한 적을 받아오는 컨텍스트
 */

struct FEnvQueryContextData;
struct FEnvQueryInstance;

UCLASS()
class CD2_API USetEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
