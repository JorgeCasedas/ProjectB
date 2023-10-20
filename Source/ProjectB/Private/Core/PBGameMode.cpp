// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/PBGameMode.h"
#include "Core/PBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

APBGameMode::APBGameMode()
{
}

void APBGameMode::BeginPlay()
{
	Super::BeginPlay();
	OnPlayerLoggedIn.Broadcast(Cast<APBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)));
}

void APBGameMode::OnPostLogin(AController* NewPlayer)
{
	OnPlayerLoggedIn.Broadcast(Cast<APBPlayerController>(NewPlayer));
}

void APBGameMode::StartPBMatch()
{
	OnMatchStarted.Broadcast();
}