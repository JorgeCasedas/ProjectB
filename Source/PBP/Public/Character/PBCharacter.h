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
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAbilitySelectionArguments
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPBGameplayAbility> Ability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> Effect;

	FAbilitySelectionArguments()
	{
	}

	FAbilitySelectionArguments(TSubclassOf<UPBGameplayAbility> InAbility)
	{
		Ability = InAbility;
	}	
	FAbilitySelectionArguments(TSubclassOf<UGameplayEffect> InEffect)
	{
		Effect = InEffect;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeathInstigator);

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
	UFUNCTION()
	void AddSelectedAbility(const TSubclassOf<UPBGameplayAbility>& Ability, const FGameplayTag& GameplayTag);
	UFUNCTION()
	void AddSelectedPassive(const TSubclassOf<UGameplayEffect>& Effect);

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLookTowardsMouse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* LastInstigator;

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
	void Death(AActor* DeathInstigator);

};

