// Fill out your copyright notice in the Description page of Project Settings.

#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/PeceptionStimuliSourceActor.h"

// Sets default values
APeceptionStimuliSourceActor::APeceptionStimuliSourceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	ActorPerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("ActorPerceptionSource"));

}

// Called when the game starts or when spawned
void APeceptionStimuliSourceActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 컴포넌트 활성화
	ActorPerceptionSource->SetActive(true);

	// Perception 항목에 이 엑터도 추가
	ActorPerceptionSource->RegisterWithPerceptionSystem();

	// 시야 감각 활성화
	ActorPerceptionSource->RegisterForSense(UAISense_Sight::StaticClass());

	Tags.Add("PerceptionActor");
}

// Called every frame
void APeceptionStimuliSourceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

