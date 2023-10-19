// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectBPlayerController.generated.h"

class AProjectCharacter;
/**
 * 
 */
UCLASS()
class PROJECTB_API AProjectBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AProjectBPlayerController();

	UFUNCTION()
	AProjectBCharacter* GetPBCharacter();
};
