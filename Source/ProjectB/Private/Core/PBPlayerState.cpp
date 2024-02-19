// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GAS/PBAbilitySystemComponent.h"
#include "GAS/PBAttributeSet.h"
#include "GAS/PBHealthAttributeSet.h"
#include "GAS/PBCombatAttributeSet.h"
#include "GameFramework/Character.h"

APBPlayerState::APBPlayerState()
{
	PBASC = CreateDefaultSubobject<UPBAbilitySystemComponent>("PBAbilitySystemComponent");
	PBASC->SetIsReplicated(true);
	PBASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NetUpdateFrequency = 100.f;

	HealthAttributeSet = CreateDefaultSubobject<UPBHealthAttributeSet>("HealthAttributeSet");

	bReplicates = true;

	bIsTheCurrentWinner = false;
}

UAbilitySystemComponent* APBPlayerState::GetAbilitySystemComponent() const
{
	return PBASC;
}

UPBAbilitySystemComponent* APBPlayerState::GetPBAbilitySystemComponent() const
{
	return PBASC;
}

void APBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APBPlayerState, TeamID);
	DOREPLIFETIME(APBPlayerState, bIsTheCurrentWinner);
}

//TODO: Test implementation change this
float APBPlayerState::GetHealth()
{
	return HealthAttributeSet->GetHealth();
}
//TODO: Test implementation change this
float APBPlayerState::GetMaxHealth()
{
	return HealthAttributeSet->GetMaxHealth();
}

bool APBPlayerState::IsEqualTo(const APlayerState* InPlayerState)
{
	return CheckUniqueNetID(InPlayerState->GetUniqueId()->ToString());
}

void APBPlayerState::SetTeamID(int InTeamID)
{
	TeamID = InTeamID;
}

int APBPlayerState::GetTeamID()
{
	return TeamID;
}

bool APBPlayerState::CheckUniqueNetID(const FString& UIDString)
{
	UE_LOG(LogTemp, Error, TEXT("Incomming %s"), *UIDString);
	UE_LOG(LogTemp, Error, TEXT("Mine %s"), *GetUniqueId()->ToString());

	if (UIDString == GetUniqueId()->ToString())
	{
		return true;
	}
	return false;
}
	 
