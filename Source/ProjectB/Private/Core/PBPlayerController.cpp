// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerController.h"
#include "Character/PBCharacter.h"
#include "Core/PBGameMode.h"
#include "SaveGame/SaveGamePlayerInfo.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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

void APBPlayerController::ClientShowPoints_Implementation(const TArray<FPlayerInfo>& PlayersInfo)
{
	if (!ScoreboardWidgetClass)
		return;

	//Set points info only on clients
	if (UGameplayStatics::GetGameMode(this) == nullptr)
	{
		USaveGamePlayerInfo* TestSaveGame = Cast<USaveGamePlayerInfo>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayerInfo::StaticClass()));
		TestSaveGame->PlayersInfo = PlayersInfo;
		UGameplayStatics::SaveGameToSlot(TestSaveGame, TEXT("TestSlot"), 0);
	}
	
	ScoreboardWidget = CreateWidget<UUserWidget>(this, ScoreboardWidgetClass);
	ScoreboardWidget->AddToViewport();
}
