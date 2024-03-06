// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerController.h"
#include "Character/PBCharacter.h"
#include "Core/PBGameMode.h"
#include "SaveGame/SaveGamePlayerInfo.h"
#include "Input/PBInputComponent.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "GAS/PBAbilitySystemComponent.h"

APBPlayerController::APBPlayerController()
{
	SetReplicates(true);
}

void APBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		if (!DefaultMappingContext || !MoveAction)
		{
			UE_LOG(LogTemp, Error, TEXT("Inputs not set correctly"));
			return;
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		bShowMouseCursor = true;
	}
}

void APBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPBInputComponent* PBInputComponent = CastChecked<UPBInputComponent>(InputComponent);
	check(PBInputComponent);

	PBInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APBPlayerController::Move);
	PBInputComponent->BindAbilityActions(InputConfig, this, &APBPlayerController::AbilityInputTagPressed, &APBPlayerController::AbilityInputTagReleased, &APBPlayerController::AbilityInputTagHeld);
}

void APBPlayerController::JoinedToMap()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("MapJoined"));
	APBGameMode* PBGameMode = Cast<APBGameMode>(GetWorld()->GetAuthGameMode());
	if (PBGameMode)
	{
		PBGameMode->OnMatchStarted.AddDynamic(this, &APBPlayerController::StartMatch);
	}

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
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Server - ControllerClientStarted"));
	ClientStartMatch();
}

void APBPlayerController::ClientStartMatch_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Client - ControllerClientStarted"));
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::DoNotLock, false, false);
	SetShowMouseCursor(true);
	OnMatchStarted.Broadcast();
}

UPBAbilitySystemComponent* APBPlayerController::GetPBASC()
{
	if (PBASC == nullptr)
	{
		PBASC = Cast< UPBAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return PBASC;
}

void APBPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!GetPBASC()) return;

	GetPBASC()->AbilityInputTagPressed(InputTag);
}

void APBPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetPBASC()) return;

	GetPBASC()->AbilityInputTagReleased(InputTag);

}

void APBPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetPBASC()) return;

	GetPBASC()->AbilityInputTagHeld(InputTag);
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
	if (!GetWorld())
		return;
	ScoreboardWidget = CreateWidget<UUserWidget>(this, ScoreboardWidgetClass,TEXT("RoundWinnersWidget"));
	ScoreboardWidget->AddToViewport();
}

void APBPlayerController::Move(const FInputActionValue& Value)
{
	
	AActor* ViewTarget = GetViewTarget();
	if (!ViewTarget)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	// Get camera rotation
	const FRotator Rotation = ViewTarget->GetActorRotation();

	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector({ 0.f, 0.f, Rotation.Yaw });
	const FVector RightDirection = UKismetMathLibrary::GetRightVector({ Rotation.Roll, 0.f, Rotation.Yaw });

	GetPBCharacter()->AddMovementInput(ForwardDirection, -MovementVector.Y);
	GetPBCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}