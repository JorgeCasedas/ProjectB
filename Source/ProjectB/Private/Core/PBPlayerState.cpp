// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerState.h"
#include "GAS/ProjectBAbilitySystemComponent.h"
#include "GAS/ProjectBAttributeSet.h"

APBPlayerState::APBPlayerState()
{
	ASC = CreateDefaultSubobject<UProjectBAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UProjectBAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* APBPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
