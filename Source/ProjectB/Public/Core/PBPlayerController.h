// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PBGameplayTags.h"
#include "InputActionValue.h"
#include "PBPlayerController.generated.h"

class AProjectCharacter;
class UPBInputConfig;
class UPBAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClientMatchStarted);

UCLASS()
class PROJECTB_API APBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APBPlayerController();

	UFUNCTION()
	APBCharacter* GetPBCharacter();
	UFUNCTION()
	void StartMatch();

	UFUNCTION(Client, Reliable)
	void ClientShowPoints(const TArray<FPlayerInfo>& PlayersInfo);

public:
	UPROPERTY(BlueprintAssignable)
	FOnClientMatchStarted OnMatchStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ScoreboardWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UUserWidget> ScoreboardWidget = nullptr;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	UFUNCTION(Client, Reliable)
	void ClientStartMatch();

private:

	UPBAbilitySystemComponent* GetPBASC();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void Move(const FInputActionValue& Value);

private:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UPBInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	TObjectPtr<UPBAbilitySystemComponent> PBASC;
};
