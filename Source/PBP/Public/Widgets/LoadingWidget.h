// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PBP_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void RemoveFromParent() override;

	UFUNCTION(BlueprintCallable)
	void CustomRemoveFromParent();
};
