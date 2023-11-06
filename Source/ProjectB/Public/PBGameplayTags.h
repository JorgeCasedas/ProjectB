// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
	PB Gameplay Tags
	Singleton containing native Gameplay Tags
*/

struct FPBGameplayTags
{
public:

	static const FPBGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Ability_Damage;
	FGameplayTag Ability_Melee;
	
	FGameplayTag Attributes_Core_Health;
	FGameplayTag Attributes_Core_MaxHealth;
	
	FGameplayTag GameplayCue_TakeDamage;

	//Inputs

	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;

protected:
private:
	static FPBGameplayTags GameplayTags;
};