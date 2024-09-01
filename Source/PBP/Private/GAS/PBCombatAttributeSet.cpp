// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBCombatAttributeSet.h"

UPBCombatAttributeSet::UPBCombatAttributeSet()
{
	InitDamage(100.f);
}

void UPBCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, Damage, COND_None, REPNOTIFY_Always);
}

void UPBCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UPBCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//FEffectProperties Props;
	//SetEffectProperties(Data, Props);
}

void UPBCombatAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, Damage, OldDamage);
}