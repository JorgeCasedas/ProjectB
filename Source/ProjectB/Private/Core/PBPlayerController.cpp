// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerController.h"
#include "Character/PBCharacter.h"
#include "Core/PBGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

APBPlayerController::APBPlayerController()
{
	SetReplicates(true);
}

void APBPlayerController::BeginPlay()
{
	APBGameMode* PBGameMode = Cast<APBGameMode>(GetWorld()->GetAuthGameMode());
	if (!PBGameMode)
	{
		return;
	}

	PBGameMode->OnMatchStarted.AddDynamic(this, &APBPlayerController::StartMatch);
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

void APBPlayerController::StartMatch()
{
	ClientStartMatch();
}

void APBPlayerController::ClientStartMatch_Implementation()
{
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::DoNotLock, false, false);
	SetShowMouseCursor(true);
	OnMatchStarted.Broadcast();
}