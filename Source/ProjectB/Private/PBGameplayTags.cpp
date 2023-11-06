// Fill out your copyright notice in the Description page of Project Settings.


#include "PBGameplayTags.h"
#include "GameplayTagsManager.h"

FPBGameplayTags FPBGameplayTags::GameplayTags;

void FPBGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Ability_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Damage"),
		FString("Damage from abilities")
	);

	GameplayTags.Ability_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Melee"),
		FString("Melee abilities")
	);

	GameplayTags.Attributes_Core_Health = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Core.Health"),
		FString("Health")
	);
	GameplayTags.Attributes_Core_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Core.MaxHealth"),
		FString("MaxHealth")
	);

	GameplayTags.GameplayCue_TakeDamage = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("GameplayCue.TakeDamage"),
		FString("Cue for taking damage")
	);


	/*
	 *		Inputs
	 */

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.Q"),
		FString("Input Tag for Q key")
	);

	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.E"),
		FString("Input Tag for E key")
	);

	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.R"),
		FString("Input Tag for R key")
	);
}