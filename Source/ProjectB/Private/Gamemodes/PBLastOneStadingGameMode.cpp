// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/PBLastOneStadingGameMode.h"
#include "Character/PBCharacter.h"
#include "core/PBPlayerState.h"
#include "SaveGame/SaveGamePlayerInfo.h"

#include "Kismet/GameplayStatics.h"

APBLastOneStadingGameMode::APBLastOneStadingGameMode()
{
}

void APBLastOneStadingGameMode::CheckWinCon()
{
	if (AliveCharacters.Num() <= 1)
	{
		GivePointsToPlayers();
	}
}

void APBLastOneStadingGameMode::GivePointsToPlayers()
{
	//PBTODO: rename when it is not a test anymore
	USaveGamePlayerInfo* TestSaveGame = Cast<USaveGamePlayerInfo>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));
	for (APBCharacter* Character : AliveCharacters)
	{
		for (FPlayerInfo& PlayerInfo : TestSaveGame->PlayersInfo)
		{
			if (PlayerInfo.UID == Character->GetPlayerState()->GetUniqueId()->ToString())
			{
				PlayerInfo.PointsWon += 1;
				break;
			}
		}
	}

	UGameplayStatics::SaveGameToSlot(TestSaveGame, TEXT("TestSlot"), 0);

	MatchFinished(TestSaveGame->PlayersInfo);
}

void APBLastOneStadingGameMode::MatchFinished(const TArray<FPlayerInfo>& PlayersInfo)
{
	Super::MatchFinished(PlayersInfo);
}
