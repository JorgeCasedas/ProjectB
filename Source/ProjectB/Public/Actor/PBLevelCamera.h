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

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitCameraStats();
	UFUNCTION()
	void OnMatchStarted();
	UFUNCTION(NetMulticast, Reliable)
	void Mulicast_OnMatchStarted();

	UFUNCTION()
	float GetMaxPlayersDistance();
	UFUNCTION()
	void RepositionCamera();
	UFUNCTION()
	void ReZoomCamera();

public:	
	virtual void Tick(float DeltaTime) override;

};
