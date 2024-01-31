// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbility.h"

void UPBGameplayAbility::SetCooldownGameplayTag(const FGameplayTag& Tag)
{
	CooldownTag = Tag;
}

void UPBGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	CooldownGE->InheritableOwnedTagsContainer.AddTag(CooldownTag);

	if (CooldownGE)
	{
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));
	}
}
