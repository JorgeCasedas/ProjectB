// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBCombatAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPBCombatAttributeSet::UPBCombatAttributeSet()
{
	InitDamage(100.f);
	InitSpeed(1.f);
	InitMovementForce(1.f);
	InitMovementResistance(1.f);

	InitLMBMaxCharges(0);
	InitRMBMaxCharges(0);
	InitQMaxCharges(0);
	InitEMaxCharges(0);
	InitRMaxCharges(0);

	InitLMBCharges(0);
	InitRMBCharges(0);
	InitQCharges(0);
	InitECharges(0);
	InitRCharges(0);

	InitFrozen(0);
}

void UPBCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, MovementForce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, MovementResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, LMBMaxCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, RMBMaxCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, QMaxCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, EMaxCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, RMaxCharges, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, LMBCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, RMBCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, QCharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, ECharges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, RCharges, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatAttributeSet, Frozen, COND_None, REPNOTIFY_Always);
}

void UPBCombatAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	//Charges min value should be 0
	if (Attribute == GetLMBChargesAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetLMBMaxCharges());
	}

	if (Attribute == GetRMBChargesAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetRMBMaxCharges());
	}

	if (Attribute == GetQChargesAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetQMaxCharges());
	}

	if (Attribute == GetEChargesAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetEMaxCharges());
	}

	if (Attribute == GetRChargesAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetRMaxCharges());
	}


	if (Attribute == GetFrozenAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, 100);
	}

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

void UPBCombatAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, Speed, OldSpeed);
}

void UPBCombatAttributeSet::OnRep_MovementForce(const FGameplayAttributeData& OldMovementForce) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, MovementForce, OldMovementForce);
}

void UPBCombatAttributeSet::OnRep_MovementResistance(const FGameplayAttributeData& OldMovementResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, MovementResistance, OldMovementResistance);
}

void UPBCombatAttributeSet::OnRep_LMBMaxCharges(const FGameplayAttributeData& OldLMBCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, LMBCharges, OldLMBCharges);
}

void UPBCombatAttributeSet::OnRep_RMBMaxCharges(const FGameplayAttributeData& OldRMBCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, RMBCharges, OldRMBCharges);
}

void UPBCombatAttributeSet::OnRep_QMaxCharges(const FGameplayAttributeData& OldQCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, QCharges, OldQCharges);
}

void UPBCombatAttributeSet::OnRep_EMaxCharges(const FGameplayAttributeData& OldECharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, ECharges, OldECharges);
}

void UPBCombatAttributeSet::OnRep_RMaxCharges(const FGameplayAttributeData& OldRCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, RCharges, OldRCharges);
}

void UPBCombatAttributeSet::OnRep_LMBCharges(const FGameplayAttributeData& OldLMBCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, LMBCharges, OldLMBCharges);
}

void UPBCombatAttributeSet::OnRep_RMBCharges(const FGameplayAttributeData& OldRMBCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, RMBCharges, OldRMBCharges);
}

void UPBCombatAttributeSet::OnRep_QCharges(const FGameplayAttributeData& OldQCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, QCharges, OldQCharges);
}

void UPBCombatAttributeSet::OnRep_ECharges(const FGameplayAttributeData& OldECharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, ECharges, OldECharges);
}

void UPBCombatAttributeSet::OnRep_RCharges(const FGameplayAttributeData& OldRCharges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, RCharges, OldRCharges);
}

void UPBCombatAttributeSet::OnRep_Frozen(const FGameplayAttributeData& OldFrozen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBCombatAttributeSet, Frozen, OldFrozen);
}