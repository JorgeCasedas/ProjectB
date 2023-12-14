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

void UPBAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UPBAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
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
const UGameplayAbility* UPBAbilitySystemComponent::GetGameplayAbilityFromInput(const FGameplayTag& InputTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTag))
		{
			return AbilitySpec.Ability;
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