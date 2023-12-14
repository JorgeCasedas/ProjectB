// Fill out your copyright notice in the Description page of Project Settings.


#include "PBGameplayTags.h"
#include "GameplayTagsManager.h"

FPBGameplayTags FPBGameplayTags::GameplayTags;

void FPBGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 *		Ability Tags
	 */
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

	GameplayTags.Ability_Active = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active"),
		FString("Active abilities")
	);

	GameplayTags.Ability_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Passive"),
		FString("Passive abilities")
	);

	/*
	 *		Active Abilities
	 */
	GameplayTags.Ability_Active_Punch = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active.Punch"),
		FString("Punch ability")
	);
	GameplayTags.Ability_Active_JumpAttack = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active.JumpAttack"),
		FString("JumpAttack ability")
	);
	GameplayTags.Ability_Active_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active.Dash"),
		FString("Dash ability")
	);
	GameplayTags.Ability_Active_Explode = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active.Explode"),
		FString("Explode ability")
	);
	GameplayTags.Ability_Active_Mistborn = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active.Mistborn"),
		FString("Mistborn ability")
	);
	GameplayTags.Ability_Active_Mistborn_NailReached = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Active.Mistborn.NailReached"),
		FString("Mistborn ability when nail reaches a target")
	);

	/*
	 *		Passive Abilities
	 */
	GameplayTags.Ability_Passive_FireTrail = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Ability.Passive.FireTrail"),
		FString("Fire trail passive")
	);

	/*
	 *		Cooldowns
	 */
	GameplayTags.Cooldown_Punch = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Cooldown.Punch"),
		FString("Punch ability cooldown")
	);

	GameplayTags.Cooldown_JumpAttack = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Cooldown.JumpAttack"),
		FString("JumpAttack ability cooldown")
	);

	GameplayTags.Cooldown_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Cooldown.Dash"),
		FString("Dash ability cooldown")
	);

	GameplayTags.Cooldown_Explode = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Cooldown.Explode"),
		FString("Explode ability cooldown")
	);
	GameplayTags.Cooldown_Mistborn = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Cooldown.Mistborn"),
		FString("Mistborn ability cooldown")
	);
	
	/*
	 *		Attributes
	 */
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

	/*
	 *		States
	 */
	GameplayTags.State_Frozen = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("State.Frozen"),
		FString("Frozen state")
	);

	GameplayTags.State_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("State.Burning"),
		FString("Burning state")
	);

	GameplayTags.State_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("State.Dead"),
		FString("Dead state")
	);

	/*
	 *		Cues
	 */
	GameplayTags.GameplayCue_TakeDamage = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("GameplayCue.TakeDamage"),
		FString("Cue for taking damage")
	);

	GameplayTags.GameplayCue_Explode = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("GameplayCue.Explode"),
		FString("Cue for exploding")
	);

	GameplayTags.GameplayCue_Death = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("GameplayCue.Death"),
		FString("Cue for dying")
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