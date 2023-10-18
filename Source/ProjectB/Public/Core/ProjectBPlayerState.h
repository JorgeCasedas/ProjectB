// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "GameFramework/PlayerState.h"
#include "ProjectBPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class PROJECTB_API AProjectBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AProjectBPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
