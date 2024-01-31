// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbility.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UPBGameplayAbility::UPBGameplayAbility()
{
	ReturnContainer.Reset();
}

void UPBGameplayAbility::SetCooldownGameplayTag(const FGameplayTag& Tag)
{
	ReturnContainer.EmptyContainer;
	CooldownTag = Tag;
	ReturnContainer = FGameplayTagContainer::EmptyContainer;
}

void UPBGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPBGameplayAbility,CooldownTag);
	DOREPLIFETIME(UPBGameplayAbility, ReturnContainer);
}

void UPBGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		if (CooldownGE  && (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo)))
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGE->GetClass(), 0);
			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->StackCount = 0;
				SpecHandle.Data->DynamicGrantedTags.AddTag(CooldownTag);
				ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
			}
		}
	}
}

const FGameplayTagContainer* UPBGameplayAbility::GetCooldownTags() const
{
	//return Super::GetCooldownTags();
	return &ReturnContainer;
}
