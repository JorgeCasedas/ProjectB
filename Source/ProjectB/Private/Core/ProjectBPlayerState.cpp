// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectBPlayerState.h"
#include "GAS/ProjectBAbilitySystemComponent.h"
#include "GAS/ProjectBAttributeSet.h"

AProjectBPlayerState::AProjectBPlayerState()
{
	ASC = CreateDefaultSubobject<UProjectBAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UProjectBAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AProjectBPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
