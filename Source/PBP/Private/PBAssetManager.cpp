// Fill out your copyright notice in the Description page of Project Settings.


#include "PBAssetManager.h"
#include "PBGameplayTags.h"

UPBAssetManager& UPBAssetManager::Get()
{
	check(GEngine);

	UPBAssetManager* PBAssetManager = Cast<UPBAssetManager>(GEngine->AssetManager);
	return *PBAssetManager;
}

void UPBAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FPBGameplayTags::InitializeNativeGameplayTags();
}