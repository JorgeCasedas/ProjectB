// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/PBGameplayAbility.h"
#include "PBGameplayTags.h"

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

void UPBAbilitySystemComponent::AddCharacterDefaultAbilities(const TArray<FDefaultCharacterAbility>& AbilitiesToAdd)
{
	for (const FDefaultCharacterAbility AbilityToAdd : AbilitiesToAdd)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityToAdd.Ability, 1);
		if (UPBGameplayAbility* Ability = Cast<UPBGameplayAbility>(AbilitySpec.Ability))
		{
#pragma region SetTags

			FGameplayTag InputTag;
			FGameplayTag CooldownTag;
			if (AbilityToAdd.InputTag.IsValid())
				InputTag = AbilityToAdd.InputTag;
			else
				InputTag = Ability->StartupInputTag;

			AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
#pragma endregion

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

void UPBAbilitySystemComponent::SetAbilitiesCooldownTags()
{
	//PBTODO: ILLEGAL CONST_CAST THIS IS AN ABOMINATION CHANGE ASAP, it works for now but AT WHAT COST? MY OWN SANITY?
	if (const UGameplayAbility* AbilityFromInputQ = GetGameplayAbilityFromInput(FPBGameplayTags::Get().InputTag_Q))
	{
		AbilityFromInputQ->GetCooldownTags();
		const UPBGameplayAbility* PBAbility = Cast<const UPBGameplayAbility>(AbilityFromInputQ);
		const_cast<UPBGameplayAbility*>(PBAbility)->SetCooldownGameplayTag(FPBGameplayTags::Get().Cooldown_Q);
	}
	if (const UGameplayAbility* AbilityFromInputE = GetGameplayAbilityFromInput(FPBGameplayTags::Get().InputTag_E))
	{
		AbilityFromInputE->GetCooldownTags();
		const UPBGameplayAbility* PBAbility = Cast<const UPBGameplayAbility>(AbilityFromInputE);
		const_cast<UPBGameplayAbility*>(PBAbility)->SetCooldownGameplayTag(FPBGameplayTags::Get().Cooldown_E);
	}
	if (const UGameplayAbility* AbilityFromInputR = GetGameplayAbilityFromInput(FPBGameplayTags::Get().InputTag_R))
	{
		AbilityFromInputR->GetCooldownTags();
		const UPBGameplayAbility* PBAbility = Cast<const UPBGameplayAbility>(AbilityFromInputR);
		const_cast<UPBGameplayAbility*>(PBAbility)->SetCooldownGameplayTag(FPBGameplayTags::Get().Cooldown_R);
	}
	if (const UGameplayAbility* AbilityFromInputRMB = GetGameplayAbilityFromInput(FPBGameplayTags::Get().InputTag_RMB))
	{
		AbilityFromInputRMB->GetCooldownTags();
		const UPBGameplayAbility* PBAbility = Cast<const UPBGameplayAbility>(AbilityFromInputRMB);
		const_cast<UPBGameplayAbility*>(PBAbility)->SetCooldownGameplayTag(FPBGameplayTags::Get().Cooldown_RMB);
	}
	if (const UGameplayAbility* AbilityFromInputLMB = GetGameplayAbilityFromInput(FPBGameplayTags::Get().InputTag_LMB))
	{
		AbilityFromInputLMB->GetCooldownTags();
		const UPBGameplayAbility* PBAbility = Cast<const UPBGameplayAbility>(AbilityFromInputLMB);
		const_cast<UPBGameplayAbility*>(PBAbility)->SetCooldownGameplayTag(FPBGameplayTags::Get().Cooldown_LMB);
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
			float InstancesNum = AbilitySpec.GetAbilityInstances().Num();
			if (InstancesNum > 0)
			{
				for (auto AbilityInstance : AbilitySpec.GetAbilityInstances())
				{
					//Cast<UPBGameplayAbility>(AbilityInstance)->input
					//if (!Cast<UPBGameplayAbility>(AbilityInstance)->CooldownTag.IsValid())
						return AbilityInstance;
				}
			}
			else 
			{
				return AbilitySpec.Ability;
			}	
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