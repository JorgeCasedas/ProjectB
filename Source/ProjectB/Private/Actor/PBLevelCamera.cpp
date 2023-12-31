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
	PlayerController->SetViewTargetWithBlend(this, 0.1f);

	APBGameMode* PBGameMode = Cast<APBGameMode>(UGameplayStatics::GetGameMode(this));
	if (!PBGameMode)
	{
		return;
	}

	PBGameMode->OnMatchStarted.AddDynamic(this, &APBLevelCamera::OnMatchStarted);
}

void APBLevelCamera::InitCameraStats()
{
	for (const APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		APBCharacter* Character = Cast<APBCharacter>(PlayerState->GetPawn());
		Characters.Add(Character);
		Character->OnDeath.AddDynamic(this, &APBLevelCamera::OnPlayerDeath);
	}

	InitArmLength = SpringArm->TargetArmLength;
	InitPlayersDistance = GetMaxPlayersDistance();
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

	for (APBCharacter* Character : Characters)
	{
		PlayersVector += Character->GetActorLocation();
	}
	FVector MidPoint = PlayersVector / Characters.Num();

	SetActorLocation(MidPoint);
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

			const float Distance = FVector::Distance(Characters[i]->GetActorLocation(), Characters[j]->GetActorLocation());
			if (Distance > CurrentPlayersDistance)
			{
				CurrentPlayersDistance = Distance;
			}
		}
	}

	return CurrentPlayersDistance;
}