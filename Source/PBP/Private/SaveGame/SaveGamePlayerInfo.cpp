// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/SaveGamePlayerInfo.h"
#include "Core/PBPlayerState.h"
#include "Online/CoreOnline.h"
#include "GAS/PBGameplayAbility.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "PBGameplayTags.h"

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
			SaveGameplayAbilities(PlayerState);
			return;
		}		
	}

	PlayersInfo.Add(FPlayerInfo(UID));	
}

void USaveGamePlayerInfo::AddPoints(APBPlayerState* PlayerState, uint8 NewPoints)
{
	FString UID = PlayerState->GetUniqueId()->ToString();

	for (FPlayerInfo& Info : PlayersInfo)
	{
		if (Info.UID == UID)
		{
			Info.PointsWon += NewPoints;
			return;
		}
	}
}

void USaveGamePlayerInfo::SaveGameplayAbilities(APBPlayerState* PlayerState)
{
	FString UID = PlayerState->GetUniqueId()->ToString();

	for (FPlayerInfo& Info : PlayersInfo)
	{
		if (Info.UID == UID)
		{
			
			check(PlayerState->GetPBAbilitySystemComponent())
			Info.GameplayAbilities.Empty();
			
			for (const FGameplayAbilitySpec AbilitySpec : PlayerState->GetPBAbilitySystemComponent()->GetActivatableAbilities())
			{
				Info.GameplayAbilities.Add(FAbilityInfo(AbilitySpec.Ability.GetClass(), AbilitySpec.DynamicAbilityTags.GetByIndex(0)));
			}
			
			return;
		}
	}
}

void USaveGamePlayerInfo::SavePlayerPassive(APBPlayerState* PlayerState, TSubclassOf<UGameplayEffect> Passive)
{
	FString UID = PlayerState->GetUniqueId()->ToString();

	for (FPlayerInfo& Info : PlayersInfo)
	{
		if (Info.UID == UID)
		{
			Info.Passives.Add(Passive);
			return;
		}
	}
}
