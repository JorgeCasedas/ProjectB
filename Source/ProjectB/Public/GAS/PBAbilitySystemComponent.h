// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PBAbilitySystemComponent.generated.h"

class UPBGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySystemComponentInitialized);
/**
 * 
 */
UCLASS()
class PROJECTB_API UPBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UPBAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	UFUNCTION(BlueprintCallable)
	void AddCharacterDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToAdd);
	UFUNCTION(BlueprintCallable)
	void AddCharacterAbility(const TSubclassOf<UGameplayAbility>& Ability, const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable)
	UClass* GetGameplayClassFromInput(const FGameplayTag& InputTag);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilitySystemComponentInitialized OnAbilitySystemComponentInitialized;

};
