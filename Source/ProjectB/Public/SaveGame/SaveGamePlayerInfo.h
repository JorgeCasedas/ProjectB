// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "GameFramework/SaveGame.h"
#include "SaveGamePlayerInfo.generated.h"

class UPBGameplayAbility
/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUniqueNetId* UID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UPBGameplayAbility>> GameplayAbilities;

	FPlayerInfo()
	{
		PointsWon = 0;
		GameplayAbilities = {};
	}

};

UCLASS()
class PROJECTB_API USaveGamePlayerInfo : public USaveGame
{
	GENERATED_BODY()
};
