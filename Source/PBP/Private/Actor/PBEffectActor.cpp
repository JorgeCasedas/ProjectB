// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PBEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/PBAttributeSet.h"
#include "PBGameplayTags.h"


APBEffectActor::APBEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));	
}

void APBEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void APBEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}
	check(GameplayEffectClass);//If gameplay effect class doesnt exist -> crash

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpedHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	EffectSpedHandle.Data.Get()->DynamicGrantedTags.AddTag(FPBGameplayTags::Get().Cooldown_E);		//SpecHandle.Data->
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpedHandle.Data.Get());
}