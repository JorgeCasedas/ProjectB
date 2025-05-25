// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "GameFramework/PlayerState.h"
#include "PBPlayerState.generated.h"


class UAbilitySystemComponent;
class UPBAbilitySystemComponent;
class UAttributeSet;
class UPBHealthAttributeSet;
class UPBCombatAttributeSet;

/**
 * 
 */
UCLASS()
class PBP_API APBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	APBPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPBAbilitySystemComponent* GetPBAbilitySystemComponent() const;
	UPBHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet; }

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	//TODO: Test implementation change this
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	//TODO: Test implementation change this
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	bool CheckUniqueNetID(const FString& UIDString);

	UFUNCTION(BlueprintCallable)
	bool IsEqualTo(const APlayerState* InPlayerState);

	UFUNCTION(BlueprintCallable)
	void SetTeamID(int TeamID);

	UFUNCTION(BlueprintCallable)
	int GetTeamID();

	UFUNCTION(BlueprintImplementableEvent)
	void UseInputGameplayEffect(TSubclassOf<UGameplayEffect> InputGameplayEffectClass);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsTheCurrentWinner = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPBAbilitySystemComponent> PBASC;

	UPROPERTY()
	TObjectPtr<UPBHealthAttributeSet> HealthAttributeSet;
	UPROPERTY()
	TObjectPtr<UPBCombatAttributeSet> CombatAttributeSet;
	

	UPROPERTY(Replicated)
	int TeamID = -1;
};
