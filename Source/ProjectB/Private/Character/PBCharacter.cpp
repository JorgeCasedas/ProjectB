// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PBCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Core/PBPlayerState.h"
#include "GAS/PBAbilitySystemComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// APBCharacter

APBCharacter::APBCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	GetCharacterMovement()->SetIsReplicated(true);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void APBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void APBCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the Client
	InitAbilityActorInfo();
}

void APBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsLocallyControlled())
		return;
	LookTowardsMouse();
}

UAbilitySystemComponent* APBCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void APBCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled())
		return;

	if(!DefaultMappingContext || !MoveAction)
	{
		UE_LOG(LogTemp, Error, TEXT("Inputs not set correctly"));
		return;
	}

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		PlayerController->bShowMouseCursor = true;
	}
}

void APBCharacter::InitAbilityActorInfo()
{
	APBPlayerState* PBPlayerState = GetPlayerState<APBPlayerState>();
	check(PBPlayerState);

	PBPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PBPlayerState, this);
	ASC = PBPlayerState->GetAbilitySystemComponent();
	AttributeSet = PBPlayerState->GetAttributeSet();
}

void APBCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APBCharacter::Move);
	}
}

void APBCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
		return;
	if (!Controller->GetViewTarget())
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	// Get camera rotation
	const FRotator Rotation = Controller->GetViewTarget()->GetActorRotation();
		
	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector({ 0.f, 0.f, Rotation.Yaw });
	const FVector RightDirection = UKismetMathLibrary::GetRightVector({ Rotation.Roll, 0.f, Rotation.Yaw });

	AddMovementInput(ForwardDirection, -MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APBCharacter::LookTowardsMouse()
{
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!GetMesh())
		return;
	if (!PlayerController)
		return;

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	float YawRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetComponentLocation(), HitResult.Location).Yaw;
	
	PlayerController->SetControlRotation({ 0.f,  YawRotation-90.f, 0.f });
}
