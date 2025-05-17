// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Core/PBGameMode.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameModesDataAsset.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FMapsGamemodesSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TSoftObjectPtr<UWorld> LobbyLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
	TEnumAsByte<EGameMode> DefaultGameMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings")
	TEnumAsByte<EWinCondition>DefaultWinCondition;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExcludedSettings")
	TArray< TEnumAsByte<EGameMode>> ExcludedGameModes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExcludedSettings")
	TArray< TEnumAsByte<EWinCondition>> ExcludedWinConditions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExcludedSettings")
	TArray< TEnumAsByte<EGameRule>> ExcludedGameRules;
};

UCLASS(BlueprintType, Blueprintable)
class PBP_API UGameModesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FExistingGameModesSettings> ExistingGameModes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMapsGamemodesSettings> SettingsPerMap;
};
