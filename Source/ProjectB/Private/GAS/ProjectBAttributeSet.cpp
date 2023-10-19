// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ProjectBAttributeSet.h"
#include "Net/UnrealNetwork.h"

UProjectBAttributeSet::UProjectBAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(GetMaxHealth());
}

void UProjectBAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UProjectBAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectBAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UProjectBAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectBAttributeSet, Health, OldHealth);
}

void UProjectBAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectBAttributeSet, MaxHealth, OldMaxHealth);
}
