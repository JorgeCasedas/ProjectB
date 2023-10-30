// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PBGameMode.h"
#include "PBLastOneStadingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API APBLastOneStadingGameMode : public APBGameMode
{
	GENERATED_BODY()
public:
	APBLastOneStadingGameMode();

	UFUNCTION()
	void PlayerDeath(APBCharacter* DeadCharacter);

	virtual void CheckWinCon() override;
	virtual void GivePointsToPlayers() override;
	virtual void MatchFinished() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APBCharacter*> DeadCharacters;
};
