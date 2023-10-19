// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectBPlayerController.h"
#include "Character/ProjectBCharacter.h"

AProjectBPlayerController::AProjectBPlayerController()
{
}

AProjectBCharacter* AProjectBPlayerController::GetPBCharacter()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return nullptr;
	}
	return Cast<AProjectBCharacter>(MyPawn);
}
