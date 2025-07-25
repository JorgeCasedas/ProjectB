// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PBGameMode.generated.h"

class UProjectBGameInstance;

class APBPlayerState;
class APBPlayerController;
class APBCharacter;

class APBBomb;

class UPBGameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedIn, APBPlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayersChangedTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerWinsGame, APBCharacter*, PlayerCharacter);

UENUM(BlueprintType)
enum EGameMode
{
	AllVsAll,
	OneVsAll,
	Teams
};

UENUM(BlueprintType)
enum EWinCondition
{
	KillAllOponents,
	PushYourOpponentOutOfTheArena,
	PassTheBomb
	//LastOneStandingLoses, I am not implementing this yet
};

UENUM(BlueprintType)
enum EGameRule
{
	FriendlyFire,
	ChangeTeamEveryXSeconds
};

USTRUCT(BlueprintType)
struct FWinConditionSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWinCondition> WinCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EGameRule>> GameRules;
};

USTRUCT(BlueprintType)
struct FExistingGameModesSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGameMode> GameMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWinConditionSettings> WinConditions;
};

USTRUCT(BlueprintType)
struct FGameModeSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGameMode> GameMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWinConditionSettings WinConditions;
};

USTRUCT(BlueprintType)
struct FRulesCompatibility
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGameRule> GameRule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EGameRule>> GameRules;
};

USTRUCT(BlueprintType)
struct FGameplayAbilitiesArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<UPBGameplayAbility>> Abilities;
	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;
};

USTRUCT(BlueprintType)
struct FPlayersByTeam
{
	GENERATED_USTRUCT_BODY()
public:
	FPlayersByTeam()
	{
		TeamID = -1;
	}
	FPlayersByTeam(int inTeamID, APBPlayerState* inState)
	{
		TeamID = inTeamID;
		Players.Add(inState);
	}
	UPROPERTY(BlueprintReadWrite)
	int TeamID;
	UPROPERTY(BlueprintReadWrite)
	TArray<APBPlayerState*> Players;
};

UCLASS(minimalapi)
class APBGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APBGameMode();

	UFUNCTION()
	void PBStartMatch();
	UFUNCTION(BlueprintCallable)
	void PBStartMatchFromBP();
	UFUNCTION()
	virtual void PBEndMatch(const TArray<FPlayerInfo>& PlayersInfo);
	UFUNCTION(BlueprintCallable)
	virtual void TravelToNextMap(const FString& LobbyPath);
	
	UFUNCTION(BlueprintCallable)
	void PlayerDeath(APBCharacter* DeadCharacter);
	UFUNCTION(BlueprintCallable)
	virtual void CheckWinCon();
	UFUNCTION()
	virtual void CheckAltBombWinCon();
	UFUNCTION()
	virtual void BombExplosion();
	UFUNCTION(BlueprintCallable)
	virtual void GivePointsToPlayers(int TeamID);

	UFUNCTION()
	virtual void GiveAbilitiesToPlayer(AController* NewPlayer);
	UFUNCTION(BlueprintCallable)
	virtual void OpenPlayerAbilitiesSelection(AController* NewPlayer, int AbilitiesToSelect);
	UFUNCTION(BlueprintCallable)
	void CharacterSelectedAbility(int SelectedAbilityIndex, const FGameplayTag& GameplayTag, APlayerController* PC);
	UFUNCTION(BlueprintCallable)
	void CharacterSelectedPassive(int SelectedAbilityIndex, APlayerController* PC);


	UFUNCTION()
	void SetPlayersTeams();
	UFUNCTION(BlueprintCallable)
	void ChangePlayersTeams();

	UFUNCTION(BlueprintCallable)
	void SetCurrentGameModeSettings(const FGameModeSettings& Settings);

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerLoggedIn OnPlayerLoggedIn;
	UPROPERTY(BlueprintAssignable)
	FOnMatchStarted OnMatchStarted;
	UPROPERTY(BlueprintAssignable)
	FOnMatchEnded OnMatchEnded;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APBCharacter*> AliveCharacters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APBCharacter*> DeadCharacters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APBCharacter*> CurrentWinners;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerWinsGame OnPlayerWinsGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxPointsToWinTheGame = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<APBPlayerController>, FGameplayAbilitiesArray> TempAbilitiesGivenToPlayers;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FPlayersByTeam> PlayersByTeams;
	UPROPERTY(BlueprintAssignable)
	FOnPlayersChangedTeam OnPlayersChangedTeam;

	UPROPERTY(BlueprintReadOnly)
	FGameModeSettings CurrentGameModeSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassTheBomb")
	TSubclassOf<APBBomb> BombClass;
	UPROPERTY(BlueprintReadOnly, Category = "PassTheBomb")
	APBBomb* BombActor;

protected:
	virtual void BeginPlay() override;
	virtual void GenericPlayerInitialization(AController* Controller) override;

private:

	UFUNCTION()
	void AllVsAllTeamsDistribution();
	UFUNCTION()
	void OneVsAllTeamsDistribution();
	UFUNCTION()
	void TeamsTeamsDistribution();

	UFUNCTION()
	void SavePlayerPassive(APBPlayerState* PS, TSubclassOf<UGameplayEffect> Effect);

private:
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectBGameInstance> PBGameInstance;
	UPROPERTY()
	int TeamCounter;
	UPROPERTY()
	FOnPlayerDeath OnPlayerDeathInternal;
	UPROPERTY()
	bool bPointsGiven = false;
};



