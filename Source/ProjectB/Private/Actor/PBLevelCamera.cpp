// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PBLevelCamera.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/PBCharacter.h"
#include "Core/PBPlayerController.h"
#include "Core/PBGameMode.h"
#include "Core/PBGameState.h"
#include "GameFramework/GameState.h"
#include "Core/PBPlayerState.h"
#include "PBGameplayTags.h"

// Sets default values
APBLevelCamera::APBLevelCamera()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SetRootComponent(SpringArm);
	SpringArm->bDoCollisionTest = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	bNetLoadOnClient = true;
}

// Called when the game starts or when spawned
void APBLevelCamera::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
	if(!PlayerController)
	{
		return;
	}
	PlayerController->SetViewTargetWithBlend(this, 0);

	APBGameMode* PBGameMode = Cast<APBGameMode>(UGameplayStatics::GetGameMode(this));
	if (!PBGameMode)
	{
		return;
	}

	PBGameMode->OnMatchStarted.AddDynamic(this, &APBLevelCamera::OnMatchStarted);
}

void APBLevelCamera::InitCameraStats()
{
	if (!GetWorld())
		return;
	if (!GetWorld()->GetGameState())
		return;

	for (const APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		APBCharacter* Character = Cast<APBCharacter>(PlayerState->GetPawn());
		Characters.Add(Character);
		Character->OnDeath.AddDynamic(this, &APBLevelCamera::OnPlayerDeath);
	}

	InitArmLength = SpringArm->TargetArmLength;
	InitPlayersDistance = GetMaxPlayersDistance();
}

void APBLevelCamera::ServerForceInitCameraStats(float maxPlayersDistance)
{
	ForceInitCameraStats(maxPlayersDistance);
	Mulicast_ForceInitCameraStats(maxPlayersDistance);
}

void APBLevelCamera::ForceInitCameraStats(float maxPlayersDistance)
{
	if (!GetWorld())
		return;
	if (!GetWorld()->GetGameState())
		return;

	for (const APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		APBCharacter* Character = Cast<APBCharacter>(PlayerState->GetPawn());
		Characters.AddUnique(Character);
	}

	InitArmLength = MaxArmLength;
	InitPlayersDistance = maxPlayersDistance;
}


void APBLevelCamera::Mulicast_ForceInitCameraStats_Implementation(float maxPlayersDistance)
{
	ForceInitCameraStats(maxPlayersDistance);
}

void APBLevelCamera::OnMatchStarted()
{
	Mulicast_OnMatchStarted();
}

void APBLevelCamera::Mulicast_OnMatchStarted_Implementation()
{
	InitCameraStats();
}

void APBLevelCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RepositionCamera();
	ReZoomCamera();
}

void APBLevelCamera::RepositionCamera()
{
	FVector PlayersVector = FVector::Zero();
	int CharactersUsed = 0;
	float MinX = 0;
	float MaxX = 0;
	bool bMinX = false;
	bool bMaxX = false;
	for (APBCharacter* Character : Characters)
	{
		if (!Character)
			continue;
		if (Character->IsHidden())
			continue;

		if (!bMinX)
		{
			bMinX = true;
			MinX = Character->GetActorLocation().X;
		}
		if (!bMaxX)
		{
			bMaxX = true;
			MaxX = Character->GetActorLocation().X;
		}

		if (MinX > Character->GetActorLocation().X)
			MinX = Character->GetActorLocation().X;
		if (MaxX < Character->GetActorLocation().X)
			MaxX = Character->GetActorLocation().X;

		PlayersVector += Character->GetActorLocation();
		CharactersUsed++;
	}
	FVector MidPoint = PlayersVector / CharactersUsed;
	//Mid point is not valid everytime because of the UI, so this helps setting an offset that solves the UI being in front of a character
	MidPoint -= OffsetDirecton * (OffsetUnits * FMath::Abs((MinX-MaxX)));
	SmoothCameraPosition(MidPoint);
}
void APBLevelCamera::SmoothCameraPosition(FVector ObjectivePosition)
{
	FVector SmoothedPosition = ObjectivePosition;
	
	if(bSmoothPosition)
		SmoothedPosition = FMath::Lerp(GetActorLocation(), ObjectivePosition, PositionSpeed);
	
	SetActorLocation(SmoothedPosition);
}
void APBLevelCamera::ReZoomCamera()
{
	if (InitPlayersDistance == 0)
	{
		SpringArm->TargetArmLength = MaxArmLength;
		return;
	}
	//Calculation based on an initial arm length adecuated to the characters spawn points;
	float ClampedZoom = FMath::Clamp(InitArmLength * (GetMaxPlayersDistance() / InitPlayersDistance), MinArmLength, MaxArmLength);
	if (!bSmoothZoom)
	{
		SpringArm->TargetArmLength = ClampedZoom;
	}
	else 
	{
		SmoothCameraZoom(ClampedZoom);
	}
}

void APBLevelCamera::SmoothCameraZoom(float TargetArmLength)
{
	TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, TargetArmLength, ZoomSpeed);
	SpringArm->TargetArmLength = TargetArmLength;
}

void APBLevelCamera::OnPlayerDeath()
{
	for (APBCharacter* Character : Characters)
	{
		if (!Character)
			continue;
		if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FPBGameplayTags::Get().State_Dead))
		{
			Characters.Remove(Character);
			break;
		}
	}
}

float APBLevelCamera::GetMaxPlayersDistance()
{
	float CurrentPlayersDistance = 0;

	for (int i = 0; i < Characters.Num(); i++)
	{
		for (int j = i+1; j < Characters.Num(); j++)
		{
			if (j >= Characters.Num())
				continue;
			if (Characters[i] == nullptr || Characters[j] == nullptr)
				continue;
			if (Characters[i]->IsHidden() || Characters[j]->IsHidden())
				continue;
			const float Distance = FVector::Distance(Characters[i]->GetActorLocation(), Characters[j]->GetActorLocation());
			if (Distance > CurrentPlayersDistance)
			{
				CurrentPlayersDistance = Distance;
			}
		}
	}

	return CurrentPlayersDistance;
}