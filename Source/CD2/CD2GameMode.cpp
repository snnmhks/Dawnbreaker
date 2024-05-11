// Copyright Epic Games, Inc. All Rights Reserved.

#include "CD2GameMode.h"
#include "Character/CDPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "CD2/Character/CDCharacterState.h"

ACD2GameMode::ACD2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Characters/Mannequins/BP_MyCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = ACDPlayerController::StaticClass();
	PlayerStateClass = ACDCharacterState::StaticClass();
}
