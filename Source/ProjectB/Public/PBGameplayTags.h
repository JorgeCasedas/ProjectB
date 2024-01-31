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
	
	//Ability Tags
	FGameplayTag Ability_Damage;
	FGameplayTag Ability_Melee; //PBTODO: Erase when changed for Ability_Active_Punch
	FGameplayTag Ability_Active;
	FGameplayTag Ability_Passive;

	//Active Abilities
	FGameplayTag Ability_Active_Punch;
	FGameplayTag Ability_Active_JumpAttack;
	FGameplayTag Ability_Active_Dash;
	FGameplayTag Ability_Active_Explode;
	FGameplayTag Ability_Active_Mistborn;
	FGameplayTag Ability_Active_Mistborn_NailReached;
	
	//Passive Abilities
	FGameplayTag Ability_Passive_FireTrail;
	
	//Cooldowns
	FGameplayTag Cooldown_Punch;
	FGameplayTag Cooldown_JumpAttack;
	FGameplayTag Cooldown_Dash;
	FGameplayTag Cooldown_Explode;
	FGameplayTag Cooldown_Mistborn;
	FGameplayTag Cooldown_Q;
	FGameplayTag Cooldown_E;
	FGameplayTag Cooldown_R;
	FGameplayTag Cooldown_RMB;
	FGameplayTag Cooldown_LMB;

	//Attributes
	FGameplayTag Attributes_Core_Health;
	FGameplayTag Attributes_Core_MaxHealth;

	//States
	FGameplayTag State_Frozen;
	FGameplayTag State_Burning;
	FGameplayTag State_Dead;
	
	//Cues
	FGameplayTag GameplayCue_TakeDamage;
	FGameplayTag GameplayCue_Explode;
	FGameplayTag GameplayCue_Death;

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