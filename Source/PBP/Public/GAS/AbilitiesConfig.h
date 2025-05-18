// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitiesConfig.generated.h"

class UPBGameplayAbility;
class UGameplayEffect;
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
	FGameplayTag CooldownTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Info;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxCharges;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ChargesEffect;
};

UCLASS()
class PBP_API UAbilitiesConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPBAbilityInfo> AbilitiesInfo;
};
