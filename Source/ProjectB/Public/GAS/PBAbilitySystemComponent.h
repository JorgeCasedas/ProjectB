// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PBAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySystemComponentInitialized);
/**
 * 
 */
UCLASS()
class PROJECTB_API UPBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilitySystemComponentInitialized OnAbilitySystemComponentInitialized;

public:
	UPBAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
};
