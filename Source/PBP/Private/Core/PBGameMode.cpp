// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PBGameMode.h"
#include "Core/PBGameState.h"
#include "Core/ProjectBGameInstance.h"
#include "Core/PBPlayerController.h"
#include "Core/PBPlayerState.h"

#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/AbilitiesConfig.h"
#include "GAS/PassivesConfig.h"

#include "Character/PBCharacter.h"

#include "Actor/PBBomb.h"

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


void APBGameMode::PBStartMatch()
{
	TArray<APlayerState*> PlayerArray = Cast<APBGameState>(UGameplayStatics::GetGameState(this))->PlayerArray;

	for (APlayerState* PS : PlayerArray)
	{
		AliveCharacters.Add(Cast<APBCharacter>(PS->GetPawn()));
	}

	PBGameInstance->SetInitialSaveGame();
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();
	for (APlayerState* PS : PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			TestSaveGame->SaveGameplayAbilities(PBPS);
		}
	}
	PBGameInstance->SavePlayersInfoSaveGame(TestSaveGame);

	SetPlayersTeams();

	bPointsGiven = false;

	OnMatchStarted.Broadcast();
}

void APBGameMode::PBStartMatchFromBP()
{
	PBStartMatch();
}

void APBGameMode::PBEndMatch(const TArray<FPlayerInfo>& PlayersInfo)
{
	check(GetWorld());
	TArray< APlayerState*> PlayerArray = Cast<APBGameState>(UGameplayStatics::GetGameState(this))->PlayerArray;

	for (APlayerState* PS : PlayerArray)
	{
		Cast<APBPlayerController>(PS->GetPlayerController())->ClientShowPoints(PlayersInfo);
	}
	OnMatchEnded.Broadcast();
}

void APBGameMode::TravelToNextMap(const FString& LobbyPath)
{
	UWorld* World = GetWorld();
	if (!World )
	{
		return;
	}
	;
	bUseSeamlessTravel = false;
	World->ServerTravel(LobbyPath + "?listen");
}

void APBGameMode::SetCurrentGameModeSettings(const FGameModeSettings& Settings)
{
	CurrentGameModeSettings = Settings;

	switch (CurrentGameModeSettings.WinConditions.WinCondition)
	{
		case EWinCondition::KillAllOponents:
			OnPlayerDeathInternal.AddDynamic(this, &APBGameMode::CheckWinCon);
		break;

		case EWinCondition::PushYourOpponentOutOfTheArena:
			OnPlayerDeathInternal.AddDynamic(this, &APBGameMode::CheckWinCon);
		break;

		case EWinCondition::PassTheBomb:
			if(BombClass)
				BombActor = GetWorld()->SpawnActor<APBBomb>(BombClass,FVector::ZeroVector,FRotator::ZeroRotator);
			if (BombActor)
			{
				BombActor->OnExploded.AddDynamic(this, &APBGameMode::BombExplosion);
				OnPlayerDeathInternal.AddDynamic(this, &APBGameMode::CheckAltBombWinCon);
			}			
		break;
	}
}

#pragma region Win Condition
void APBGameMode::PlayerDeath(APBCharacter* DeadCharacter)
{
	if (!AliveCharacters.Contains(DeadCharacter))
		return;

	AliveCharacters.Remove(DeadCharacter);
	DeadCharacters.Add(DeadCharacter);
	OnPlayerDeathInternal.Broadcast();
}

void APBGameMode::CheckWinCon()
{
	int LastTeamID = -1;
	switch (CurrentGameModeSettings.GameMode)
	{
		case EGameMode::AllVsAll:
			if (AliveCharacters.Num() > 1)
			{
				return;
			}
		break;

		case EGameMode::OneVsAll:
			//Check Teams Left
			for (APBCharacter* Character : AliveCharacters)
			{
				int CheckTeamID = Cast<APBPlayerState>(Character->GetPlayerState())->GetTeamID();
				if (LastTeamID < 0)
					LastTeamID = CheckTeamID;
				else if (LastTeamID != CheckTeamID)
					return;
			}
		break;

		case EGameMode::Teams:
			//Check Teams Left
			for (APBCharacter* Character : AliveCharacters)
			{
				int CheckTeamID = Cast<APBPlayerState>(Character->GetPlayerState())->GetTeamID();
				if (LastTeamID < 0)
					LastTeamID = CheckTeamID;
				else if (LastTeamID != CheckTeamID)
					return;
			}
		break;
	}
	GivePointsToPlayers(LastTeamID);
}

void APBGameMode::CheckAltBombWinCon() 
{
	if (AliveCharacters.Num() < 2)
	{
		GivePointsToPlayers(-1);
	}
	else if (CurrentGameModeSettings.GameMode == EGameMode::Teams)
	{
		int LastTeamID = -1;
		for (APBCharacter* Character : AliveCharacters)
		{
			int CheckTeamID = Cast<APBPlayerState>(Character->GetPlayerState())->GetTeamID();
			if (LastTeamID < 0)
				LastTeamID = CheckTeamID;
			else if (LastTeamID != CheckTeamID)
				return;
		}
		GivePointsToPlayers(-1);
	}
}

void APBGameMode::BombExplosion()
{
	GivePointsToPlayers(-1);
}

void APBGameMode::GivePointsToPlayers(int TeamID)
{
	if (bPointsGiven)
		return;

	bPointsGiven = true;
	//PBTODO: rename when it is not a test anymore
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();

	if (TeamID < 0)
	{
		CurrentWinners = AliveCharacters;
	}
	else 
	{
		for (APBCharacter* Character : AliveCharacters)
		{
			if (Cast<APBPlayerState>(Character->GetPlayerState())->GetTeamID() == TeamID)
				CurrentWinners.AddUnique(Character);
		}
		for (APBCharacter* Character : DeadCharacters)
		{
			if (Cast<APBPlayerState>(Character->GetPlayerState())->GetTeamID() == TeamID)
				CurrentWinners.AddUnique(Character);
		}
	}

	for (APBCharacter* Character : CurrentWinners)
	{
		for (FPlayerInfo& PlayerInfo : TestSaveGame->PlayersInfo)
		{
			if (PlayerInfo.UID == Character->GetPlayerState()->GetUniqueId()->ToString())
			{
				PlayerInfo.PointsWon += 1;
				Cast<APBPlayerState>(Character->GetPlayerState())->bIsTheCurrentWinner = true;
				if (PlayerInfo.PointsWon >= MaxPointsToWinTheGame)
				{
					OnPlayerWinsGame.Broadcast(Character);
				}
				break;
			}
		}
	}

	PBGameInstance->SavePlayersInfoSaveGame(TestSaveGame);

	PBEndMatch(TestSaveGame->PlayersInfo);
}

#pragma endregion

#pragma region Teams Distribution
void APBGameMode::SetPlayersTeams()
{
	TeamCounter = 0;
	switch (CurrentGameModeSettings.GameMode)
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

void APBGameMode::ChangePlayersTeams()
{
	//PBTODO: change this logic so left players dont just wait until they are matched together and win the game
	TeamCounter = 0;
	switch (CurrentGameModeSettings.GameMode)
	{
		case OneVsAll:
			OneVsAllTeamsDistribution();
			break;

		case Teams:
			TeamsTeamsDistribution();
			break;
	}
	switch (CurrentGameModeSettings.WinConditions.WinCondition)
	{
		case EWinCondition::KillAllOponents:
			CheckWinCon();
		break;

		case EWinCondition::PushYourOpponentOutOfTheArena:
			CheckWinCon();
		break;
	}
	OnPlayersChangedTeam.Broadcast();
}

void APBGameMode::AllVsAllTeamsDistribution()
{
	TArray<APlayerState*> PlayersList = UGameplayStatics::GetGameState(this)->PlayerArray;
	// Shuffle the list
	const int32 NumShuffles = PlayersList.Num() - 1;
	for (int32 i = 0; i < NumShuffles; ++i)
	{
		int32 SwapIdx = FMath::RandRange(i, NumShuffles);
		PlayersList.Swap(i, SwapIdx);
	}

	for (FPlayersByTeam& Players : PlayersByTeams)
	{
		Players.Players.Empty();
	}

	for (APlayerState* PS : PlayersList)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			PBPS->SetTeamID(TeamCounter);

			bool bTeamFound = false;
			for (FPlayersByTeam& Players : PlayersByTeams)
			{
				if (Players.TeamID == TeamCounter)
				{
					bTeamFound = true;
					Players.Players.Add(PBPS);
				}
			}
			if (!bTeamFound)
			{
				PlayersByTeams.Add({ TeamCounter, PBPS });
			}

			TeamCounter++;
		}
	}
}

void APBGameMode::OneVsAllTeamsDistribution()
{
	int RandomPlayer = FMath::RandRange(0, UGameplayStatics::GetGameState(this)->PlayerArray.Num() - 1);
	int PlayerIteratorCounter = 0;

	for (FPlayersByTeam& Players : PlayersByTeams)
	{
		Players.Players.Empty();
	}

	for (APlayerState* PS : UGameplayStatics::GetGameState(this)->PlayerArray)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			int SelectedTeam = 0;
			if (PlayerIteratorCounter == RandomPlayer)
				SelectedTeam = 1;

			PBPS->SetTeamID(SelectedTeam);

			bool bTeamFound = false;
			for (FPlayersByTeam& Players : PlayersByTeams)
			{
				if (Players.TeamID == SelectedTeam)
				{
					bTeamFound = true;
					Players.Players.Add(PBPS);
				}
			}
			if (!bTeamFound)
			{
				PlayersByTeams.Add({ SelectedTeam, PBPS });
			}

			PlayerIteratorCounter++;
		}
	}
}

void APBGameMode::TeamsTeamsDistribution()
{
	TArray<APlayerState*> PlayersList = UGameplayStatics::GetGameState(this)->PlayerArray;
	// Shuffle the list
	const int32 NumShuffles = PlayersList.Num() - 1;
	for (int32 i = 0; i < NumShuffles; ++i)
	{
		int32 SwapIdx = FMath::RandRange(i, NumShuffles);
		PlayersList.Swap(i, SwapIdx);
	}

	int MaxPlayersPerTeam = 2;
	int PlayersAssigned = 0;

	for (FPlayersByTeam& Players : PlayersByTeams)
	{
		Players.Players.Empty();
	}

	for (APlayerState* PS : PlayersList)
	{
		if (APBPlayerState* PBPS = Cast<APBPlayerState>(PS))
		{
			PBPS->SetTeamID(TeamCounter);

			bool bTeamFound = false;
			for (FPlayersByTeam& Players : PlayersByTeams)
			{
				if (Players.TeamID == TeamCounter)
				{
					bTeamFound = true;
					Players.Players.Add(PBPS);
				}
			}
			if (!bTeamFound)
			{
				PlayersByTeams.Add({ TeamCounter, PBPS });
			}

			PlayersAssigned++;
			if (PlayersAssigned >= MaxPlayersPerTeam)
			{
				PlayersAssigned = 0;
				TeamCounter++;
			}
		}
	}
}
void APBGameMode::SavePlayerPassive(APBPlayerState* PS, TSubclassOf<UGameplayEffect> Effect)
{
	PBGameInstance->SetInitialSaveGame();
	USaveGamePlayerInfo* TestSaveGame = PBGameInstance->GetPlayerInfoSaveGame();
	TestSaveGame->SavePlayerPassive(PS, Effect);
	PBGameInstance->SavePlayersInfoSaveGame(TestSaveGame);
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
				for (const TSubclassOf<UGameplayEffect> Effect : PlayerInfo.Passives)
				{
					PBPS->GetPBAbilitySystemComponent()->AddCharacterPassive(Effect);
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
		check(PBGameInstance->PassivesDataAsset);

		TArray<FAbilitySelectionArguments> AbilitiesForSelection;
		FGameplayAbilitiesArray AbilitiesArray;

		int AblitiesNumber = PBGameInstance->AbilitiesDataAsset->AbilitiesInfo.Num();
		int EffectsNumber = PBGameInstance->PassivesDataAsset->EffectsInfo.Num();

		int TotalUpgradesNumber = AblitiesNumber + EffectsNumber;

		if (AbilitiesToSelect > TotalUpgradesNumber)
		{
			AbilitiesToSelect = TotalUpgradesNumber;
		}

		TArray<int> AbilitiesIndexes;

		for (int i = 0; AbilitiesIndexes.Num() < AbilitiesToSelect; i++) {

			AbilitiesIndexes.AddUnique(FMath::RandRange(0, TotalUpgradesNumber - 1));
		}

		for (int index : AbilitiesIndexes)
		{
			if (index >= AblitiesNumber)
			{
				int EffectIndex = index - AblitiesNumber;
				AbilitiesForSelection.Add({ PBGameInstance->PassivesDataAsset->EffectsInfo[EffectIndex].EffectClass});
				AbilitiesArray.PassiveEffects.Add({ PBGameInstance->PassivesDataAsset->EffectsInfo[EffectIndex].EffectClass });
			}
			else 
			{
				AbilitiesForSelection.Add({ PBGameInstance->AbilitiesDataAsset->AbilitiesInfo[index].AbilityClass });
				AbilitiesArray.Abilities.Add({ PBGameInstance->AbilitiesDataAsset->AbilitiesInfo[index].AbilityClass });
			}
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
void APBGameMode::CharacterSelectedPassive(int SelectedAbilityIndex, APlayerController* PC)
{
	for (const TPair <TObjectPtr<APBPlayerController>, FGameplayAbilitiesArray> Pair : TempAbilitiesGivenToPlayers)
	{
		if (PC == Pair.Key)
		{
			int i = 0;
			for (const TSubclassOf<UGameplayEffect> Effect : Pair.Value.PassiveEffects)
			{
				if (SelectedAbilityIndex == i)
				{
					Cast<APBCharacter>(PC->GetPawn())->AddSelectedPassive(Effect);
					SavePlayerPassive(Cast<APBPlayerState>(PC->PlayerState), Effect);
				}
				i++;
			}
		}
	}
}
#pragma endregion