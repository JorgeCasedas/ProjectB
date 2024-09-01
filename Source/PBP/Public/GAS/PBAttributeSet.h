// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PBAttributeSet.generated.h"

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC;
	UPROPERTY()
	AActor* SourceAvatarActor;
	UPROPERTY()
	AController* SourceController;
	UPROPERTY()
	ACharacter* SourceCharacter;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC;
	UPROPERTY()
	AActor* TargetAvatarActor;
	UPROPERTY()
	AController* TargetController;
	UPROPERTY()
	ACharacter* TargetCharacter;
};

/**
 * 
 */
UCLASS()
class PBP_API UPBAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPBAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
