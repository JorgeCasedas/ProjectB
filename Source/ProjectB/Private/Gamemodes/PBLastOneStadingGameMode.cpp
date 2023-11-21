// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/PBLastOneStadingGameMode.h"
#include "Character/PBCharacter.h"
#include "Core/PBPlayerState.h"
#include "Core/PBPlayerController.h"
#include "SaveGame/SaveGamePlayerInfo.h"
#include "Core/ProjectBGameInstance.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/AbilitiesConfig.h"
#include "GAS/PBGameplayAbility.h"

#include "GameFramework/GameStateBase.h"
#include "Abilities/GameplayAbility.h"

#include "Kismet/GameplayStatics.h"

APBLastOneStadingGameMode::APBLastOneStadingGameMode()
{
}

void APBLastOneStadingGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnMatchStarted.AddDynamic(this, &APBLastOneStadingGameMode::MatchStarted);

	PBGameInstance = Cast<UProjectBGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void APBLastOneStadingGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	GiveAbilitiesToPlayer(NewPlayer);
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
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();
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

	PBGameInstance->SavePlayersInfoSaveGame(TestSaveGame);

	MatchFinished(TestSaveGame->PlayersInfo);
}

void APBLastOneStadingGameMode::MatchFinished(const TArray<FPlayerInfo>& PlayersInfo)
{
	Super::MatchFinished(PlayersInfo);
}

void APBLastOneStadingGameMode::GiveAbilitiesToPlayer(AController* NewPlayer)
{
	if (!PBGameInstance)
	{
		PBGameInstance = Cast<UProjectBGameInstance>(UGameplayStatics::GetGameInstance(this));
	}
	if (!PBGameInstance->bSaveGameAlreadyCreated)
		return;
	if (!NewPlayer)
		return;
	if (!NewPlayer->PlayerState)
		return;
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();
	if (!TestSaveGame)
		return;

	if (APBPlayerState* PBPS = Cast<APBPlayerState>(NewPlayer->PlayerState))
	{
		for (const FPlayerInfo PlayerInfo : TestSaveGame->PlayersInfo)
		{
			const FString DebugSavedUID = PlayerInfo.UID;
			const FString DebugExistingUID = PBPS->GetUniqueId()->ToString();

			if (PBPS->GetUniqueId()->ToString() == PlayerInfo.UID)
			{
				for (const FAbilityInfo& AbilityInfo : PlayerInfo.GameplayAbilities)
				{
					PBPS->GetPBAbilitySystemComponent()->AddCharacterAbility(TSubclassOf<UGameplayAbility>(AbilityInfo.Ability), AbilityInfo.InputTag);
				}
			}
		}
	}
	OpenPlayerAbilitiesSelection(NewPlayer, 3);
}

void APBLastOneStadingGameMode::OpenPlayerAbilitiesSelection(AController* NewPlayer, int AbilitiesToSelect)
{
	if (APBCharacter* Character = Cast< APBCharacter>(NewPlayer->GetPawn()))
	{
		check(PBGameInstance);
		check(PBGameInstance->AbilitiesDataAsset);

		TArray<FAbilitySelectionArguments> AbilitiesForSelection;
		FGameplayAbilitiesArray AbilitiesArray;

		int AblitiesNumber = PBGameInstance->AbilitiesDataAsset->AbilitiesInfo.Num();

		if (AbilitiesToSelect > AblitiesNumber)
		{
			AbilitiesToSelect = AblitiesNumber;
		}

		TArray<int> AbilitiesIndexes;

		for (int i = 0; AbilitiesIndexes.Num() < AbilitiesToSelect; i++) {

			AbilitiesIndexes.AddUnique(FMath::RandRange(0, AblitiesNumber-1));
		}
		
		for (int index : AbilitiesIndexes)
		{
			AbilitiesForSelection.Add({PBGameInstance->AbilitiesDataAsset->AbilitiesInfo[index].AbilityClass });
			AbilitiesArray.Abilities.Add({ PBGameInstance->AbilitiesDataAsset->AbilitiesInfo[index].AbilityClass });
		}

		TempAbilitiesGivenToPlayers.Add(Cast<APBPlayerController>(NewPlayer), AbilitiesArray);
		Character->ClientOpenAbilitiesSelection(AbilitiesForSelection);
	}
	else 
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &APBLastOneStadingGameMode::OpenPlayerAbilitiesSelection, NewPlayer, AbilitiesToSelect);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
	}
}

void APBLastOneStadingGameMode::MatchStarted()
{
	PBGameInstance->SetInitialSaveGame();
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();
	for (APlayerState* PS: UGameplayStatics::GetGameState(this)->PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			TestSaveGame->SaveGameplayAbilities(PBPS);
		}
	}
	PBGameInstance->SavePlayersInfoSaveGame(TestSaveGame);
}