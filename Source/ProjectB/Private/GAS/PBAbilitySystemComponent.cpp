// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/PBGameplayAbility.h"

UPBAbilitySystemComponent::UPBAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UPBAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	OnAbilitySystemComponentInitialized.Broadcast();	
}

void UPBAbilitySystemComponent::AddCharacterDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToAdd)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToAdd)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UPBGameplayAbility* Ability = Cast<UPBGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UPBAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UGameplayAbility>& Ability, const FGameplayTag& InputTag)
{
	//First -> Clear any ability binded to the input
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTag))
		{
			ClearAbility(AbilitySpec.Handle);
			break;
		}		
	}
	//Second -> Add and bind the new ability
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
	if (Cast<UPBGameplayAbility>(AbilitySpec.Ability))
	{
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		GiveAbility(AbilitySpec);
	}
}

UClass* UPBAbilitySystemComponent::GetGameplayClassFromInput(const FGameplayTag& InputTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTag))
		{
			return AbilitySpec.Ability->GetClass();
		}
	}

	return nullptr;
}
void UPBAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Spec);
			if (!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}
void UPBAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	/*if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec Spec : GetActivatableAbilities())
	{
		if(Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Spec);
			if(!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}*/
}

void UPBAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(Spec);
		}
	}
}