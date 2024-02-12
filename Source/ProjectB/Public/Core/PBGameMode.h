// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PBGameMode.generated.h"

class APBPlayerController;
class APBCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedIn, APBPlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);

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

USTRUCT()
struct FGameModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte < EGameMode> GameMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte < EWinCondition> WinCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte < EGameRule> GameRule;
};

UCLASS(minimalapi)
class APBGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APBGameMode();

	UFUNCTION(BlueprintCallable)
	void StartPBMatch();
	UFUNCTION(BlueprintCallable)
	void PlayerDeath(APBCharacter* DeadCharacter);

	UFUNCTION(BlueprintCallable)
	virtual void CheckWinCon();
	UFUNCTION(BlueprintCallable)
	virtual void GivePointsToPlayers();
	UFUNCTION(BlueprintCallable)
	virtual void MatchFinished(const TArray<FPlayerInfo>& PlayersInfo);
	UFUNCTION(BlueprintCallable)
	virtual void TravelToNextMap();

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerLoggedIn OnPlayerLoggedIn;
	UPROPERTY(BlueprintAssignable)
	FOnMatchStarted OnMatchStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APBCharacter*> AliveCharacters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APBCharacter*> DeadCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> NextLevel;

	UPROPERTY(BlueprintReadWrite) //PBTODO: change to FGameModeSettings
	TEnumAsByte<EGameMode> CurrentGameMode = EGameMode::Teams;

protected:
	virtual void GenericPlayerInitialization(AController* Controller) override;
};



