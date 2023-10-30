// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/PBLastOneStadingGameMode.h"

APBLastOneStadingGameMode::APBLastOneStadingGameMode()
{
}

void APBLastOneStadingGameMode::PlayerDeath(APBCharacter* DeadCharacter)
{
	AliveCharacters.Remove(DeadCharacter);
	DeadCharacters.Add(DeadCharacter);
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
	//PBTODO: Give points to alive character if any
	MatchFinished();
}

void APBLastOneStadingGameMode::MatchFinished()
{
	TravelToNextMap();
}
