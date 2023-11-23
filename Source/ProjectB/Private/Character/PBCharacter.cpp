// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PBCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Core/PBPlayerState.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/PBHealthAttributeSet.h"

#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// APBCharacter

APBCharacter::APBCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	GetCharacterMovement()->SetIsReplicated(true);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void APBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//GetMesh()->bOnlyAllowAutonomousTickPose = false;

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

void APBCharacter::ClientOpenAbilitiesSelection_Implementation(const TArray<FAbilitySelectionArguments>& AbilitiesToSelectFrom)
{
	OpenAbilitiesSelection(AbilitiesToSelectFrom);
}

void APBCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APBCharacter::InitAbilityActorInfo()
{
	APBPlayerState* PBPlayerState = GetPlayerState<APBPlayerState>();
	check(PBPlayerState);

	ASC = PBPlayerState->GetPBAbilitySystemComponent();
	check(ASC)

	ASC->InitAbilityActorInfo(PBPlayerState, this);
	AttributeSet = PBPlayerState->GetHealthAttributeSet();
	check(AttributeSet);
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &APBCharacter::HealthChanged);
}

void APBCharacter::HealthChanged(const FOnAttributeChangeData& HealthData)
{
	if (HealthData.NewValue <= 0)
	{
		Death();
	}
}

void APBCharacter::Death()
{
	OnDeath.Broadcast();
}

void APBCharacter::LookTowardsMouse()
{
	if (!bLookTowardsMouse)
		return;

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!GetMesh())
		return;
	if (!PlayerController)
		return;

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	float YawRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetComponentLocation(), HitResult.Location).Yaw;
	
	PlayerController->SetControlRotation({ 0.f,  YawRotation, 0.f });
}
