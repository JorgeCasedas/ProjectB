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

void UPBAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToAdd)
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

void UPBAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

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
	}
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