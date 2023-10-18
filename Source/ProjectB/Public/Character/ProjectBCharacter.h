// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ProjectBCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(config=Game)
class AProjectBCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectBCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void Tick(float DeltaSeconds) override;
	
#pragma region Getters
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY()
		TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
		TObjectPtr<UAttributeSet> AttributeSet;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	
	virtual void BeginPlay();

	void Move(const FInputActionValue& Value);
	void LookTowardsMouse();

private:
	void InitAbilityActorInfo();
};

