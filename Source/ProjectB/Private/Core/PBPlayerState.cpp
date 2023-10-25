// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBPlayerState.h"
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

	AttributeSetRename = CreateDefaultSubobject<UPBHealthAttributeSet>("HealthAttributeSet");
	//UE_LOG(LogTemp, Error, TEXT("Create Player State"));
	//CreateDefaultSubobject<UPBCombatAttributeSet>("CombatAttributeSet");
}

UAbilitySystemComponent* APBPlayerState::GetAbilitySystemComponent() const
{
	return PBASC;
}

UPBAbilitySystemComponent* APBPlayerState::GetPBAbilitySystemComponent() const
{
	return PBASC;
}

//TODO: Test implementation change this
float APBPlayerState::GetHealth()
{
	return Cast<UPBHealthAttributeSet>(AttributeSetRename) ? Cast<UPBHealthAttributeSet>(AttributeSetRename)->GetHealth() : 0;
}
//TODO: Test implementation change this
float APBPlayerState::GetMaxHealth()
{
	return Cast<UPBHealthAttributeSet>(AttributeSetRename) ? Cast<UPBHealthAttributeSet>(AttributeSetRename)->GetMaxHealth() : 0;
}

void APBPlayerState::BeginPlay()
{
	Super::BeginPlay();
	AActor* OwnerActor = GetOwner();

	if (!PBASC) return;
	if (!PBASC->GetAvatarActor()) return;
	if (!Cast<ACharacter>(PBASC->GetAvatarActor())) return;
	bool bLocallyControlled = Cast<ACharacter>(PBASC->GetAvatarActor())->IsLocallyControlled();
	if (HasAuthority() && bLocallyControlled)
	{
		//ASC->RemoveAllSpawnedAttributes();
		//ASC->RemoveSpawnedAttribute(AttributeSet);
		
	}
}
