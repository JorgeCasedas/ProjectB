// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PBGameState.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS()
class PBP_API APBGameState : public AGameState
{
	GENERATED_BODY()
	//TODO: Recieve OnMatchStart from Game mode so clients can subscribe to it

public:
	UFUNCTION(BlueprintCallable)
	void SetMapWalkSounds(const TArray<USoundBase*>& InWalkSounds);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<USoundBase*> CurrentWalkSounds;
};
