// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbility.h"

void UPBGameplayAbility::SetCooldownGameplayEffectClass(TSubclassOf<class UGameplayEffect> CooldownClass)
{
	CooldownGameplayEffectClass = CooldownClass;
}
