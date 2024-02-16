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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedIn, APBPlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

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
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<TSubclassOf<UPBGameplayAbility>> Abilities;
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
	virtual void TravelToNextMap();
	
	UFUNCTION(BlueprintCallable)
	void PlayerDeath(APBCharacter* DeadCharacter);
	UFUNCTION(BlueprintCallable)
	virtual void CheckWinCon();
	UFUNCTION()
	virtual void BombExplosion();
	UFUNCTION(BlueprintCallable)
	virtual void GivePointsToPlayers(int TeamID);

	UFUNCTION()
	virtual void GiveAbilitiesToPlayer(AController* NewPlayer);
	UFUNCTION()
	virtual void OpenPlayerAbilitiesSelection(AController* NewPlayer, int AbilitiesToSelect);
	UFUNCTION(BlueprintCallable)
	void CharacterSelectedAbility(int SelectedAbilityIndex, const FGameplayTag& GameplayTag, APlayerController* PC);

	UFUNCTION()
	void SetPlayersTeams();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> NextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<APBPlayerController>, FGameplayAbilitiesArray> TempAbilitiesGivenToPlayers;
	
	UPROPERTY()
	TMap<int, APBPlayerState*> PlayersByTeams;

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

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameModeSettings CurrentGameModeSettings;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectBGameInstance> PBGameInstance;
	UPROPERTY()
	int TeamCounter;
	UPROPERTY()
	FOnPlayerDeath OnPlayerDeathInternal;
};



