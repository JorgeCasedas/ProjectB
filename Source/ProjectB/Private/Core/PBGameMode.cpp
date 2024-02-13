// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PBGameMode.h"
#include "Core/PBGameState.h"
#include "Core/ProjectBGameInstance.h"
#include "Core/PBPlayerController.h"
#include "Core/PBPlayerState.h"

#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/AbilitiesConfig.h"

#include "Character/PBCharacter.h"

#include "SaveGame/SaveGamePlayerInfo.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerState.h"


APBGameMode::APBGameMode()
{
}

void APBGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnMatchStarted.AddDynamic(this, &APBGameMode::MatchStarted);

	PBGameInstance = Cast<UProjectBGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void APBGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);
	APBPlayerController* PBPlayerController = Cast<APBPlayerController>(Controller);
	OnPlayerLoggedIn.Broadcast(PBPlayerController);
	PBPlayerController->JoinedToMap();
	GiveAbilitiesToPlayer(Controller);
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

void APBGameMode::MatchStarted()
{
	TeamCounter = 0;
	PBGameInstance->SetInitialSaveGame();
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();
	for (APlayerState* PS : UGameplayStatics::GetGameState(this)->PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			TestSaveGame->SaveGameplayAbilities(PBPS);
		}
	}
	PBGameInstance->SavePlayersInfoSaveGame(TestSaveGame);

	SetPlayersTeams();
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

#pragma region Win Condition
void APBGameMode::PlayerDeath(APBCharacter* DeadCharacter)
{
	AliveCharacters.Remove(DeadCharacter);
	DeadCharacters.Add(DeadCharacter);
	CheckWinCon();
}

void APBGameMode::CheckWinCon()
{
	if (AliveCharacters.Num() <= 1)
	{
		GivePointsToPlayers();
	}
}

void APBGameMode::GivePointsToPlayers()
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

#pragma endregion

#pragma region Teams Distribution
void APBGameMode::SetPlayersTeams()
{
	switch (CurrentGameMode)
	{
	case AllVsAll:
		AllVsAllTeamsDistribution();
		break;

	case OneVsAll:
		OneVsAllTeamsDistribution();
		break;

	case Teams:
		TeamsTeamsDistribution();
		break;
	}
}

void APBGameMode::AllVsAllTeamsDistribution()
{
	UGameplayStatics::GetGameState(this)->PlayerArray.Num();

	for (APlayerState* PS : UGameplayStatics::GetGameState(this)->PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			PBPS->SetTeamID(TeamCounter);
			PlayersByTeams.Add(TeamCounter, PBPS);
			TeamCounter++;
		}
	}
}

void APBGameMode::OneVsAllTeamsDistribution()
{
	int RandomPlayer = FMath::RandRange(0, UGameplayStatics::GetGameState(this)->PlayerArray.Num() - 1);
	int PlayerIteratorCounter = 0;
	for (APlayerState* PS : UGameplayStatics::GetGameState(this)->PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			int SelectedTeam = 0;
			if (PlayerIteratorCounter == RandomPlayer)
				SelectedTeam = 1;

			PBPS->SetTeamID(SelectedTeam);
			PlayersByTeams.Add(SelectedTeam, PBPS);

			PlayerIteratorCounter++;
		}
	}
}

void APBGameMode::TeamsTeamsDistribution()
{
	int MaxPlayersPerTeam = 2;
	int PlayersAssigned = 0;

	for (APlayerState* PS : UGameplayStatics::GetGameState(this)->PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			PBPS->SetTeamID(TeamCounter);
			PlayersByTeams.Add(TeamCounter, PBPS);

			PlayersAssigned++;
			if (PlayersAssigned >= MaxPlayersPerTeam)
			{
				PlayersAssigned = 0;
				TeamCounter++;
			}
		}
	}
}
#pragma endregion

#pragma region Abilities Management

void APBGameMode::GiveAbilitiesToPlayer(AController* NewPlayer)
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

void APBGameMode::OpenPlayerAbilitiesSelection(AController* NewPlayer, int AbilitiesToSelect)
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

			AbilitiesIndexes.AddUnique(FMath::RandRange(0, AblitiesNumber - 1));
		}

		for (int index : AbilitiesIndexes)
		{
			AbilitiesForSelection.Add({ PBGameInstance->AbilitiesDataAsset->AbilitiesInfo[index].AbilityClass });
			AbilitiesArray.Abilities.Add({ PBGameInstance->AbilitiesDataAsset->AbilitiesInfo[index].AbilityClass });
		}

		TempAbilitiesGivenToPlayers.Add(Cast<APBPlayerController>(NewPlayer), AbilitiesArray);
		Character->ClientOpenAbilitiesSelection(AbilitiesForSelection);
	}
	else
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &APBGameMode::OpenPlayerAbilitiesSelection, NewPlayer, AbilitiesToSelect);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
	}
}

void APBGameMode::CharacterSelectedAbility(int SelectedAbilityIndex, const FGameplayTag& GameplayTag, APlayerController* PC)
{
	for (const TPair <TObjectPtr<APBPlayerController>, FGameplayAbilitiesArray> Pair : TempAbilitiesGivenToPlayers)
	{
		if (PC == Pair.Key)
		{
			int i = 0;
			for (const TSubclassOf<UPBGameplayAbility> Ability : Pair.Value.Abilities)
			{
				if (SelectedAbilityIndex == i)
				{
					Cast<APBCharacter>(PC->GetPawn())->AddSelectedAbility(Ability, GameplayTag);
				}
				i++;
			}
		}
	}
}
#pragma endregion