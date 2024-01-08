// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PBGameMode.h"
#include "PBLastOneStadingGameMode.generated.h"

class APBPlayerController;
class UPBGameplayAbility;
class UProjectBGameInstance;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FGameplayAbilitiesArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<UPBGameplayAbility>> Abilities;
};

UCLASS()
class PROJECTB_API APBLastOneStadingGameMode : public APBGameMode
{
	GENERATED_BODY()
public:
	APBLastOneStadingGameMode();

	virtual void CheckWinCon() override;
	virtual void GivePointsToPlayers() override;
	virtual void MatchFinished(const TArray<FPlayerInfo>& PlayersInfo) override;
	
	UFUNCTION()
	virtual void GiveAbilitiesToPlayer(AController* NewPlayer);
	UFUNCTION()
	virtual void OpenPlayerAbilitiesSelection(AController* NewPlayer, int AbilitiesToSelect);
	UFUNCTION()
	virtual void MatchStarted();

	UFUNCTION(BlueprintCallable)
	void CharacterSelectedAbility(int SelectedAbilityIndex, const FGameplayTag& GameplayTag, APlayerController* PC);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<APBPlayerController>, FGameplayAbilitiesArray> TempAbilitiesGivenToPlayers;

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectBGameInstance> PBGameInstance;
};
