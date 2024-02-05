// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbility.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UPBGameplayAbility::UPBGameplayAbility()
{
}

void UPBGameplayAbility::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPBGameplayAbility, CooldownTag);
}

void UPBGameplayAbility::SetCooldownGameplayTag(const FGameplayTag& Tag)
{
	CooldownTag = Tag;
}

//void UPBGameplayAbility::OnRep_CooldownTag()
//{
//
//}

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
	//Sometimes CooldownTag is not set, when pressing the input for the ability this function gets called 2-3 times one of the times is correctly set but that last time it
	//is called CooldownTag is empty, this only happends on the client so i guess it is a replication problem, server client works fine now
	//This behaviour is seen only after the first server travel (havent try with a build and connecting like if it was a real game, only with UE editor)
	UE_LOG(LogTemp, Error, TEXT("%i -> %i -> %s"),GetWorld()->IsClient() , GetUniqueID(), *CooldownTag.GetTagName().ToString());
	return &CooldownTag.GetSingleTagContainer();
}
