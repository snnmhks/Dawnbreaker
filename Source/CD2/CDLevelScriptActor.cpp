// Fill out your copyright notice in the Description page of Project Settings.

#include "Barricade/Barricade.h"
#include "CDLevelScriptActor.h"

void ACDLevelScriptActor::BeginPlay()
{
	Barricades = GetWorld()->SpawnActor<ABarricade>();
}
