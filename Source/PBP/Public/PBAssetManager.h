// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PBAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PBP_API UPBAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UPBAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
};
