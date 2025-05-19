// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PBGameplayAbility.h"
#include "PBGameplayAbilityWithCharges.generated.h"

/**
 * 
 */
UCLASS()
class PBP_API UPBGameplayAbilityWithCharges : public UPBGameplayAbility
{
	GENERATED_BODY()
	
	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** Called when the ability is removed from an AbilitySystemComponent */
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
};
