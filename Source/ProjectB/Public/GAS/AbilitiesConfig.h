// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitiesConfig.generated.h"

class UPBGameplayAbility;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FPBAbilityInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPBGameplayAbility> AbilityClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Info;
};

UCLASS()
class PROJECTB_API UAbilitiesConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPBAbilityInfo> AbilitiesInfo;
};
