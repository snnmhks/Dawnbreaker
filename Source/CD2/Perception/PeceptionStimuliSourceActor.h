// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C:\Users\snnmh\Desktop\CD2\Source\CD2\Interface\IdentificationInterface.h"
#include "PeceptionStimuliSourceActor.generated.h"

class AActor;
class IIdentificationPlayer;

UCLASS()
class CD2_API APeceptionStimuliSourceActor : public AActor, public IIdentificationPlayer
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APeceptionStimuliSourceActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
	class UAIPerceptionStimuliSourceComponent* ActorPerceptionSource;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};