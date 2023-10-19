// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ProjectBLevelCamera.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/ProjectBCharacter.h"

// Sets default values
AProjectBLevelCamera::AProjectBLevelCamera()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SetRootComponent(SpringArm);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectBLevelCamera::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
	if(PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(this, 0.1f);
	}

	//TODO: Wait for all players to be connected
	InitCameraStats();
}

void AProjectBLevelCamera::InitCameraStats()
{
	InitArmLength = SpringArm->TargetArmLength;
	InitPlayersDistance = GetMaxPlayersDistance();
}

// Called every frame
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
		return;
	}
	//Calculation based on an initial arm length adecuated to the characters spawn points;
	float UnClampedZoom = InitArmLength * (GetMaxPlayersDistance() / InitPlayersDistance);
	SpringArm->TargetArmLength = FMath::Clamp(UnClampedZoom, MinArmLength, MaxArmLength);
}

float AProjectBLevelCamera::GetMaxPlayersDistance()
{
	float CurrentPlayersDistance = 0;
	//TODO: Improve this nested for 
	for (const AProjectBCharacter* CharacterA : Characters)
	{
		for (const AProjectBCharacter* CharacterB : Characters)
		{
			const float Distance = FVector::Distance(CharacterA->GetActorLocation(), CharacterB->GetActorLocation());
			if (Distance > CurrentPlayersDistance)
			{
				CurrentPlayersDistance = Distance;
			}
		}
	}
	return CurrentPlayersDistance;
}

