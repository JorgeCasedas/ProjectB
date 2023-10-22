// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PBPlayerController.generated.h"

class AProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClientMatchStarted);

UCLASS()
class PROJECTB_API APBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APBPlayerController();

	UFUNCTION()
	APBCharacter* GetPBCharacter();
	UFUNCTION()
	void StartMatch();

public:
	UPROPERTY(BlueprintAssignable)
	FOnClientMatchStarted OnMatchStarted;

protected:
	virtual void BeginPlay();

	UFUNCTION(Client, Reliable)
	void ClientStartMatch();
};
