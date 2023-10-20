// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerState.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/PBAttributeSet.h"

APBPlayerState::APBPlayerState()
{
	ASC = CreateDefaultSubobject<UPBAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPBAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* APBPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
