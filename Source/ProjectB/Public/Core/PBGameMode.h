// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PBGameMode.generated.h"

class AProjectBPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedIn, AProjectBPlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);


UCLASS(minimalapi)
class APBGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APBGameMode();

	UFUNCTION(BlueprintCallable)
	void StartPBMatch();

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerLoggedIn OnPlayerLoggedIn;
	UPROPERTY(BlueprintAssignable)
	FOnMatchStarted OnMatchStarted;

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
};



