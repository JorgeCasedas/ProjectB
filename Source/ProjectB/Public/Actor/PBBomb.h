// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBBomb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExploded);

UCLASS()
class PROJECTB_API APBBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBBomb();

public:
	UPROPERTY()
	FOnExploded OnExploded;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
