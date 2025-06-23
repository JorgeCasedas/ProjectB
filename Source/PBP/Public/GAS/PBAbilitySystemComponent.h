// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PBGameplayAbility.h" //not compiling without this, forward declaraton not working?
#include "PBAbilitySystemComponent.generated.h"

//class UPBGameplayAbility;

USTRUCT(BlueprintType)
struct FDefaultCharacterAbility
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UPBGameplayAbility> Ability;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag InputTag;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySystemComponentInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityFailedToActivateDueToCooldown);
/**
 * 
 */
UCLASS()
class PBP_API UPBAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UPBAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual UGameplayAbility* CreateNewInstanceOfAbility(FGameplayAbilitySpec& Spec, const UGameplayAbility* Ability) override;

	UFUNCTION(BlueprintCallable)
	void AddCharacterDefaultAbilities(const TArray<FDefaultCharacterAbility>& AbilitiesToAdd);
	UFUNCTION(BlueprintCallable)
	void AddCharacterAbility(const TSubclassOf<UGameplayAbility>& Ability, const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable)
	void AddCharacterPassive(const TSubclassOf<UGameplayEffect>& Effect);
	UFUNCTION()
	void SetCooldownBasedOnInput(const FGameplayTag& Input, FGameplayTag& CooldownTagRef);
	UFUNCTION(BlueprintCallable)
	UClass* GetGameplayClassFromInput(const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable)
	const UGameplayAbility* GetGameplayAbilityFromInput(const FGameplayTag& InputTag);

	void AbilityInputTagPressed(const FGameplayTag& InputTag, TSubclassOf<UGameplayEffect> InputGameplayEffectClass);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilitySystemComponentInitialized OnAbilitySystemComponentInitialized;
	UPROPERTY(BlueprintAssignable)
	FOnAbilityFailedToActivateDueToCooldown OnAbilityFailedToActivateDueToCooldown;

};
