// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PBGameplayTags.h"
#include "PBGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API UPBGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPBGameplayAbility();

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	//This is only intended to be used when the gameplay abilities are being set up not while being in the gameplay phase of the game
	UFUNCTION(BlueprintCallable)
	void SetCooldownGameplayTag(const FGameplayTag& Tag);

	/*UFUNCTION()
	void OnRep_CooldownTag();*/

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FGameplayTag CdTag;
};
