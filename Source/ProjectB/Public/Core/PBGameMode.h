// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PBGameMode.generated.h"

class APBPlayerController;
class APBCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedIn, APBPlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);

//TODO: Deactivate player starts on use (so every player has a different spawn point)
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

protected:
	virtual void GenericPlayerInitialization(AController* Controller) override;
};



