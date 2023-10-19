// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ProjectBGameMode.h"
#include "Core/ProjectBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AProjectBGameMode::AProjectBGameMode()
{
}

void AProjectBGameMode::BeginPlay()
{
	Super::BeginPlay();
	OnPlayerLoggedIn.Broadcast(Cast<AProjectBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)));
}

void AProjectBGameMode::OnPostLogin(AController* NewPlayer)
{
	OnPlayerLoggedIn.Broadcast(Cast<AProjectBPlayerController>(NewPlayer));
}

void AProjectBGameMode::StartMatch()
{
	OnMatchStarted.Broadcast();
}