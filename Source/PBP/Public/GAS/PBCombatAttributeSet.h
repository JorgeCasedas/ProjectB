// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PBAttributeSet.h"
#include "PBCombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class PBP_API UPBCombatAttributeSet : public UPBAttributeSet
{
	GENERATED_BODY()

public:
	UPBCombatAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue)const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Damage Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LMBMaxCharges, Category = "Damage Attributes")
	FGameplayAttributeData LMBMaxCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, LMBMaxCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RMBMaxCharges, Category = "Damage Attributes")
	FGameplayAttributeData RMBMaxCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, RMBMaxCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QMaxCharges, Category = "Damage Attributes")
	FGameplayAttributeData QMaxCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, QMaxCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EMaxCharges, Category = "Damage Attributes")
	FGameplayAttributeData EMaxCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, EMaxCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RMaxCharges, Category = "Damage Attributes")
	FGameplayAttributeData RMaxCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, RMaxCharges);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LMBCharges, Category = "Damage Attributes")
	FGameplayAttributeData LMBCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, LMBCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RMBCharges, Category = "Damage Attributes")
	FGameplayAttributeData RMBCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, RMBCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QCharges, Category = "Damage Attributes")
	FGameplayAttributeData QCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, QCharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ECharges, Category = "Damage Attributes")
	FGameplayAttributeData ECharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, ECharges);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RCharges, Category = "Damage Attributes")
	FGameplayAttributeData RCharges;
	ATTRIBUTE_ACCESSORS(UPBCombatAttributeSet, RCharges);

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;

	UFUNCTION()
	void OnRep_LMBMaxCharges(const FGameplayAttributeData& OldLMBCharges) const;
	UFUNCTION()
	void OnRep_RMBMaxCharges(const FGameplayAttributeData& OldRMBCharges) const;
	UFUNCTION()
	void OnRep_QMaxCharges(const FGameplayAttributeData& OldQCharges) const;
	UFUNCTION()
	void OnRep_EMaxCharges(const FGameplayAttributeData& OldECharges) const;
	UFUNCTION()
	void OnRep_RMaxCharges(const FGameplayAttributeData& OldRCharges) const;

	UFUNCTION()
	void OnRep_LMBCharges(const FGameplayAttributeData& OldLMBCharges) const;
	UFUNCTION()
	void OnRep_RMBCharges(const FGameplayAttributeData& OldRMBCharges) const;
	UFUNCTION()
	void OnRep_QCharges(const FGameplayAttributeData& OldQCharges) const;
	UFUNCTION()
	void OnRep_ECharges(const FGameplayAttributeData& OldECharges) const;
	UFUNCTION()
	void OnRep_RCharges(const FGameplayAttributeData& OldRCharges) const;
};
