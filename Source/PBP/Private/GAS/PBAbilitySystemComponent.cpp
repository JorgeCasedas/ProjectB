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

UGameplayAbility* UPBAbilitySystemComponent::CreateNewInstanceOfAbility(FGameplayAbilitySpec& Spec, const UGameplayAbility* Ability)
{
	UGameplayAbility* AbilityInstance = Super::CreateNewInstanceOfAbility(Spec, Ability);
	if (UPBGameplayAbility* PBAbilityInstace = Cast<UPBGameplayAbility>(AbilityInstance))
	{
		SetCooldownBasedOnInput(Spec.DynamicAbilityTags.GetByIndex(0), PBAbilityInstace->CdTag);
	}
	
	return AbilityInstance;
}

void UPBAbilitySystemComponent::AddCharacterDefaultAbilities(const TArray<FDefaultCharacterAbility>& AbilitiesToAdd)
{
	for (const FDefaultCharacterAbility& AbilityToAdd : AbilitiesToAdd)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityToAdd.Ability, 1);
		if (UPBGameplayAbility* Ability = Cast<UPBGameplayAbility>(AbilitySpec.Ability))
		{
#pragma region SetTags

			FGameplayTag InputTag;
			//FGameplayTag CooldownTag;
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

void UPBAbilitySystemComponent::SetCooldownBasedOnInput(const FGameplayTag& Input, FGameplayTag& CooldownTagRef)
{
	if (Input == FPBGameplayTags::Get().InputTag_Q)
	{
		CooldownTagRef = FPBGameplayTags::Get().Cooldown_Q;
	}
	else if (Input == FPBGameplayTags::Get().InputTag_E)
	{
		CooldownTagRef = FPBGameplayTags::Get().Cooldown_E;
	}
	else if (Input == FPBGameplayTags::Get().InputTag_R)
	{
		CooldownTagRef = FPBGameplayTags::Get().Cooldown_R;
	}
	else if (Input == FPBGameplayTags::Get().InputTag_RMB)
	{
		CooldownTagRef = FPBGameplayTags::Get().Cooldown_RMB;
	}
	else if (Input == FPBGameplayTags::Get().InputTag_LMB)
	{
		CooldownTagRef = FPBGameplayTags::Get().Cooldown_LMB;
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
void UPBAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag, TSubclassOf<UGameplayEffect> InputGameplayEffectClass)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Spec);
			if (!Spec.IsActive())
			{
				bool bAbilityWasActivated =	TryActivateAbility(Spec.Handle);
				if (bAbilityWasActivated && InputGameplayEffectClass !=nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Activated"));
					BP_ApplyGameplayEffectToSelf(InputGameplayEffectClass, 0, MakeEffectContext());
				}
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