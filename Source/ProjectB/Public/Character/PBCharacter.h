// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PBCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UPBHealthAttributeSet;

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

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;
	//TODO: Find a better way to have this input action in a DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY()
		TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
		TObjectPtr<UPBHealthAttributeSet> AttributeSet;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	
	virtual void BeginPlay();

	void Move(const FInputActionValue& Value);
	void LookTowardsMouse();

private:
	UFUNCTION()
	void InitAbilityActorInfo();

	virtual void HealthChanged(const FOnAttributeChangeData& HealthData);
	UFUNCTION()
	void Death();
};

