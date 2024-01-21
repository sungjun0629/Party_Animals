// Copyright Epic Games, Inc. All Rights Reserved.

#include "animal_gangGameMode.h"
#include "animal_gangCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aanimal_gangGameMode::Aanimal_gangGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
