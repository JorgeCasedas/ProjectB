// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PBBomb.h"

// Sets default values
APBBomb::APBBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APBBomb::BeginPlay()
{
	Super::BeginPlay();
	
}