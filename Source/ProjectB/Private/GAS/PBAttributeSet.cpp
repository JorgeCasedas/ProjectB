// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPBAttributeSet::UPBAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(GetMaxHealth());
}

void UPBAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPBAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBAttributeSet, Health, OldHealth);
}

void UPBAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBAttributeSet, MaxHealth, OldMaxHealth);
}
