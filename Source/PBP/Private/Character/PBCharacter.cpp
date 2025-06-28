// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PBCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayEffectExtension.h"

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

void APBCharacter::AddSelectedAbility(const TSubclassOf<UPBGameplayAbility>& Ability, const FGameplayTag& GameplayTag)
{
	const TSubclassOf<UGameplayAbility> DefaultAblityClass = Ability;
	Cast<APBPlayerState>(GetPlayerState())->GetPBAbilitySystemComponent()->AddCharacterAbility(DefaultAblityClass, GameplayTag);

}

void APBCharacter::AddSelectedPassive(const TSubclassOf<UGameplayEffect>& Effect)
{
	Cast<APBPlayerState>(GetPlayerState())->GetPBAbilitySystemComponent()->AddCharacterPassive(Effect);
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
	AActor* HealthLoseInstigator = HealthData.GEModData ? HealthData.GEModData->EffectSpec.GetEffectContext().GetInstigator() : nullptr;
	if (HealthData.NewValue <= 0)
	{
		Death(HealthLoseInstigator);
	}
	else 
	{
		LastInstigator = HealthLoseInstigator;
	}
}

void APBCharacter::Death(AActor* DeathInstigator)
{
	OnDeath.Broadcast(DeathInstigator);
	bLookTowardsMouse = false;
}

void APBCharacter::LookTowardsMouse()
{
	if (!bLookTowardsMouse)
		return;

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!GetMesh())
		return;

	if (GetAbilitySystemComponent())
		if(GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Frozen")))
			return;

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	FHitResult HitResult;
	if (!PlayerController)
		return;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);
	if (!GetMesh())
		return;
	float YawRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetComponentLocation(), HitResult.Location).Yaw;
	
	PlayerController->SetControlRotation({ 0.f,  YawRotation, 0.f });
}
