// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ProjectBLevelCamera.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/ProjectBCharacter.h"
#include "Core/PBPlayerController.h"
#include "Core/PBGameMode.h"
#include "Core/PBGameState.h"
#include "GameFramework/GameState.h"
#include "Core/PBPlayerState.h"

// Sets default values
AProjectBLevelCamera::AProjectBLevelCamera()
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
void AProjectBLevelCamera::BeginPlay()
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

	PBGameMode->OnMatchStarted.AddDynamic(this, &AProjectBLevelCamera::OnMatchStarted);
}

void AProjectBLevelCamera::InitCameraStats()
{
	for (const APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		Characters.Add(Cast<AProjectBCharacter>(PlayerState->GetPawn()));
	}

	InitArmLength = SpringArm->TargetArmLength;
	InitPlayersDistance = GetMaxPlayersDistance();
}

void AProjectBLevelCamera::OnMatchStarted()
{
	Mulicast_OnMatchStarted();
}

void AProjectBLevelCamera::Mulicast_OnMatchStarted_Implementation()
{
	InitCameraStats();
}

void AProjectBLevelCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RepositionCamera();
	ReZoomCamera();
}

void AProjectBLevelCamera::RepositionCamera()
{
	FVector PlayersVector = FVector::Zero();

	for (AProjectBCharacter* Character : Characters)
	{
		PlayersVector += Character->GetActorLocation();
	}
	FVector MidPoint = PlayersVector / Characters.Num();

	SetActorLocation(MidPoint);
}

void AProjectBLevelCamera::ReZoomCamera()
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

float AProjectBLevelCamera::GetMaxPlayersDistance()
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