// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProjectBGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API AProjectBGameState : public AGameState
{
	GENERATED_BODY()
	//TODO: Recieve OnMatchStart from Game mode so clients can subscribe to it
};
