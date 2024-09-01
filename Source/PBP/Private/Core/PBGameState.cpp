// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PBGameState.h"
#include "Sound/SoundBase.h"

void APBGameState::SetMapWalkSounds(const TArray<USoundBase*>& InWalkSounds)
{
	CurrentWalkSounds = InWalkSounds;
}
