// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ProjectBCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Core/ProjectBPlayerState.h"
#include "GAS/ProjectBAbilitySystemComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AProjectBCharacter

AProjectBCharacter::AProjectBCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AProjectBCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AProjectBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsLocallyControlled())
		return;
	LookTowardsMouse();
}

UAbilitySystemComponent* AProjectBCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AProjectBCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocallyControlled())
		return;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AProjectBCharacter::InitAbilityActorInfo()
{
	AProjectBPlayerState* ProjectBPlayerState = GetPlayerState<AProjectBPlayerState>();
	check(ProjectBPlayerState);

	ProjectBPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ProjectBPlayerState, this);
	ASC = ProjectBPlayerState->GetAbilitySystemComponent();
	AttributeSet = ProjectBPlayerState->GetAttributeSet();
}

void AProjectBCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectBCharacter::Move);
	}
}

void AProjectBCharacter::Move(const FInputActionValue& Value)
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

void AProjectBCharacter::LookTowardsMouse()
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
	
	GetMesh()->SetRelativeRotation({ 0.f,  YawRotation-90.f, 0.f });
}
