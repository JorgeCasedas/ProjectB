// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBGameMode.h"
#include "ProjectBCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectBGameMode::AProjectBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
