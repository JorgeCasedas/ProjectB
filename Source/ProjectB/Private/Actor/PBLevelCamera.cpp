// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PBLevelCamera.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/PBCharacter.h"
#include "Core/PBPlayerController.h"
#include "Core/PBGameMode.h"
#include "Core/PBGameState.h"
#include "GameFramework/GameState.h"
#include "Core/PBPlayerState.h"

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
		Characters.Add(Cast<APBCharacter>(PlayerState->GetPawn()));
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
	float UnClampedZoom = InitArmLength * (GetMaxPlayersDistance() / InitPlayersDistance);
	SpringArm->TargetArmLength = FMath::Clamp(UnClampedZoom, MinArmLength, MaxArmLength);
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