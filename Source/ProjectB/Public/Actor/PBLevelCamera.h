// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBLevelCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
class APBCharacter;
class APBPlayerController;

UCLASS()
class PROJECTB_API APBLevelCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	APBLevelCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinArmLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float InitPlayersDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<APBCharacter>> Characters;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector OffsetDirecton = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OffsetUnits = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSmoothZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.01", ClampMax = "1.0", UIMin = "0.01", UIMax = "1.0"))
	float ZoomSpeed;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitCameraStats();
	UFUNCTION()
	void OnMatchStarted();
	UFUNCTION(NetMulticast, Reliable)
	void Mulicast_OnMatchStarted();
	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	float GetMaxPlayersDistance();
	UFUNCTION()
	void RepositionCamera();
	UFUNCTION()
	void ReZoomCamera();
	UFUNCTION()
	void SmoothCameraZoom(float TargetArmLength);

public:	
	virtual void Tick(float DeltaTime) override;

};
