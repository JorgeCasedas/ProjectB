// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "GameFramework/Character.h"
#include "PBCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UPBHealthAttributeSet;
class UPBGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilitySelectionArguments
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPBGameplayAbility> Ability;

	FAbilitySelectionArguments()
	{
	}

	FAbilitySelectionArguments(TSubclassOf<UPBGameplayAbility> InAbility)
	{
		Ability = InAbility;
	}	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(config=Game)
class APBCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APBCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void Tick(float DeltaSeconds) override;
	
#pragma region Getters
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPBHealthAttributeSet* GetAttributeSet() const { return AttributeSet; }
#pragma endregion

	UFUNCTION(Reliable, Client, BlueprintCallable)
	void ClientOpenAbilitiesSelection(const TArray<FAbilitySelectionArguments>& AbilitiesToSelectFrom);

	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true))
	void OpenAbilitiesSelection(const TArray<FAbilitySelectionArguments>& AbilitiesToSelectFrom);

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

protected:
	UPROPERTY()
		TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
		TObjectPtr<UPBHealthAttributeSet> AttributeSet;

protected:
	virtual void BeginPlay();

	void LookTowardsMouse();

private:
	UFUNCTION()
	void InitAbilityActorInfo();

	virtual void HealthChanged(const FOnAttributeChangeData& HealthData);
	UFUNCTION()
	void Death();

};

