// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectBGameMode.generated.h"

class AProjectBPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLoggedIn, AProjectBPlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);


UCLASS(minimalapi)
class AProjectBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectBGameMode();

	UFUNCTION(BlueprintCallable)
	void StartMatch();

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerLoggedIn OnPlayerLoggedIn;
	UPROPERTY(BlueprintAssignable)
	FOnMatchStarted OnMatchStarted;

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
};



