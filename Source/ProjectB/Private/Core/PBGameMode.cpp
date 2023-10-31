// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PBGameMode.h"
#include "Core/PBPlayerController.h"
#include "Core/PBGameState.h"
#include "Character/PBCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"


APBGameMode::APBGameMode()
{
}

void APBGameMode::BeginPlay()
{
	Super::BeginPlay();
	OnPlayerLoggedIn.Broadcast(Cast<APBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)));
}

void APBGameMode::OnPostLogin(AController* NewPlayer)
{
	OnPlayerLoggedIn.Broadcast(Cast<APBPlayerController>(NewPlayer));
}

void APBGameMode::StartPBMatch()
{
	TArray< APlayerState*> PlayerArray = Cast<APBGameState>(UGameplayStatics::GetGameState(this))->PlayerArray;
	for(APlayerState* PS : PlayerArray)
	{
		AliveCharacters.Add(Cast<APBCharacter>(PS->GetPawn()));
	}
	OnMatchStarted.Broadcast();
}

void APBGameMode::PlayerDeath(APBCharacter* DeadCharacter)
{
	AliveCharacters.Remove(DeadCharacter);
	DeadCharacters.Add(DeadCharacter);
}

void APBGameMode::CheckWinCon()
{
	MatchFinished();
}

void APBGameMode::GivePointsToPlayers()
{
}

void APBGameMode::MatchFinished()
{
	TravelToNextMap();
}

void APBGameMode::TravelToNextMap()
{
	UWorld* World = GetWorld();
	if (!World || !NextLevel)
	{
		return;
	}

	bUseSeamlessTravel = false;

	FString LobbyPath = NextLevel.GetLongPackageName();
	World->ServerTravel(LobbyPath + "?listen");
}
