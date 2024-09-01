// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "SaveGamePlayerInfo.generated.h"

class UPBGameplayAbility;
class APBPlayerState;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPBGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InputTag = FGameplayTag();

	FAbilityInfo()
	{
	}

	FAbilityInfo(TSubclassOf<UPBGameplayAbility> InAbility, FGameplayTag InInputTag)
	{
		Ability = InAbility;
		InputTag = InInputTag;
	}
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PointsWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityInfo> GameplayAbilities;

	FPlayerInfo()
	{
		UID = "INVALID";
		PointsWon = 0;
	}

	FPlayerInfo(FString& NewUID)
	{
		UID = NewUID;
		PointsWon = 0;
	}

};

UCLASS()
class PBP_API USaveGamePlayerInfo : public USaveGame
{
	GENERATED_BODY()
public:
	USaveGamePlayerInfo();

	UFUNCTION(BlueprintCallable)
	void AddPlayerToInfo(APBPlayerState* PlayerState);
	UFUNCTION(BlueprintCallable)
	void AddPoints(APBPlayerState* PlayerState, uint8 NewPoints);
	UFUNCTION(BlueprintCallable)
	void SaveGameplayAbilities(APBPlayerState* PlayerState);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerInfo> PlayersInfo;
};
