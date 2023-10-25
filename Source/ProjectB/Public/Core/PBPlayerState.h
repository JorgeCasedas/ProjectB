// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "GameFramework/PlayerState.h"
#include "PBPlayerState.generated.h"


class UAbilitySystemComponent;
class UPBAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class PROJECTB_API APBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	APBPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPBAbilitySystemComponent* GetPBAbilitySystemComponent() const;
	UAttributeSet* GetAttributeSet() const { return AttributeSetRename; }
	
	//TODO: Test implementation change this
	UFUNCTION(BlueprintCallable)
		float GetHealth();
	//TODO: Test implementation change this
	UFUNCTION(BlueprintCallable)
		float GetMaxHealth();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPBAbilitySystemComponent> ASCRename2;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSetRename;
	UPROPERTY()
	TObjectPtr<UAttributeSet> CombatAttributeSet;

protected:
	virtual void BeginPlay() override;
};
