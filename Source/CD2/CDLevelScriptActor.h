// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CDLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class CD2_API ACDLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Barricade")
	ABarricade* Barricades;
	
protected:
	virtual void BeginPlay() override;
};
