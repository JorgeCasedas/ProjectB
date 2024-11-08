// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PBExecution.generated.h"

/**
 * 
 */
UCLASS()
class PBP_API UPBExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UPBExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
