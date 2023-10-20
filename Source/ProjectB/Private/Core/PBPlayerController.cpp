// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerController.h"
#include "Character/ProjectBCharacter.h"

APBPlayerController::APBPlayerController()
{
}

AProjectBCharacter* APBPlayerController::GetPBCharacter()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return nullptr;
	}
	return Cast<AProjectBCharacter>(MyPawn);
}
