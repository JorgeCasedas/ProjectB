// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerController.h"
#include "Character/PBCharacter.h"

APBPlayerController::APBPlayerController()
{
}

APBCharacter* APBPlayerController::GetPBCharacter()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return nullptr;
	}
	return Cast<APBCharacter>(MyPawn);
}
