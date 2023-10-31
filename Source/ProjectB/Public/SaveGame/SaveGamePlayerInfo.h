// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGamePlayerInfo.generated.h"

class UPBGameplayAbility;
class APBPlayerState;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PointsWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UPBGameplayAbility>> GameplayAbilities;

	FPlayerInfo()
	{
		UID = "INVALID";
		PointsWon = 0;
		GameplayAbilities = {};
	}

	FPlayerInfo(FString& NewUID)
	{
		UID = NewUID;
		PointsWon = 0;
		GameplayAbilities = {};
	}

};

UCLASS()
class PROJECTB_API USaveGamePlayerInfo : public USaveGame
{
	GENERATED_BODY()
public:
	USaveGamePlayerInfo();

	UFUNCTION(BlueprintCallable)
	void AddPlayerToInfo(APBPlayerState* PlayerState);
	UFUNCTION(BlueprintCallable)
	void AddPoints(APBPlayerState* PlayerState, uint8 NewPoints);
	UFUNCTION(BlueprintCallable)
	void AddGameplayAbilities(APBPlayerState* PlayerState, TArray<TSubclassOf<UPBGameplayAbility>> NewGameplayAbilities);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerInfo> PlayersInfo;
};
