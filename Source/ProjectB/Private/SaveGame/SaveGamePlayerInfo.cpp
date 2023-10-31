// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/SaveGamePlayerInfo.h"
#include "Core/PBPlayerState.h"
#include "Online/CoreOnline.h"
#include "GAS/PBGameplayAbility.h"

USaveGamePlayerInfo::USaveGamePlayerInfo()
{

}

void USaveGamePlayerInfo::AddPlayerToInfo(APBPlayerState* PlayerState)
{
	FString UID = PlayerState->GetUniqueId()->ToString();
	
	for (FPlayerInfo Info : PlayersInfo) 
	{
		if (Info.UID == UID)
		{
			//User already has info
			return;
		}		
	}

	PlayersInfo.Add(FPlayerInfo(UID));
}

void USaveGamePlayerInfo::AddPoints(APBPlayerState* PlayerState, uint8 NewPoints)
{
	FString UID = PlayerState->GetUniqueId()->ToString();

	for (FPlayerInfo Info : PlayersInfo)
	{
		if (Info.UID == UID)
		{
			Info.PointsWon += NewPoints;
			return;
		}
	}
}

void USaveGamePlayerInfo::AddGameplayAbilities(APBPlayerState* PlayerState, TArray<TSubclassOf<UPBGameplayAbility>> NewGameplayAbilities)
{
	FString UID = PlayerState->GetUniqueId()->ToString();

	for (FPlayerInfo Info : PlayersInfo)
	{
		if (Info.UID == UID)
		{
			for (TSubclassOf<UPBGameplayAbility> GAClass : NewGameplayAbilities)
			{
				Info.GameplayAbilities.AddUnique(GAClass);
			}
			return;
		}
	}
}