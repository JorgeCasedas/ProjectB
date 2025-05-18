// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbilityWithCharges.h"
#include "Core/PBPlayerController.h"

void UPBGameplayAbilityWithCharges::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	Cast<APBPlayerController>(ActorInfo->PlayerController)->OnAbilityAdded(this);
}

void UPBGameplayAbilityWithCharges::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	Cast<APBPlayerController>(ActorInfo->PlayerController)->OnAbilityRemoved(this);
}
