// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PBGameMode.h"
#include "Core/PBPlayerController.h"
#include "Core/PBGameState.h"
#include "Character/PBCharacter.h"
#include "SaveGame/SaveGamePlayerInfo.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"


APBGameMode::APBGameMode()
{
}

void APBGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);
	APBPlayerController* PBPlayerController = Cast<APBPlayerController>(Controller);
	OnPlayerLoggedIn.Broadcast(PBPlayerController);
	PBPlayerController->JoinedToMap();
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
	CheckWinCon();
}

void APBGameMode::CheckWinCon()
{
	//Make to force implement in children
}

void APBGameMode::GivePointsToPlayers()
{
	//Make to force implement in children
}

void APBGameMode::MatchFinished(const TArray<FPlayerInfo>& PlayersInfo)
{
	check(GetWorld());
	TArray< APlayerState*> PlayerArray = Cast<APBGameState>(UGameplayStatics::GetGameState(this))->PlayerArray;

	for (APlayerState* PS : PlayerArray)
	{
		Cast<APBPlayerController>(PS->GetPlayerController())->ClientShowPoints(PlayersInfo);
	}
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
