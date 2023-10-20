// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PBEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/PBAttributeSet.h"
#include "Components/CapsuleComponent.h"

APBEffectActor::APBEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
}

void APBEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: make it work with GAS
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UPBAttributeSet* PBAttributeSet = Cast<UPBAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UPBAttributeSet::StaticClass()));

		if (PBAttributeSet)
		{
			//TODO: This is hack, so big NONO, change it as soon as possible
			UPBAttributeSet* MutablePBAttributeSet = const_cast<UPBAttributeSet*>(PBAttributeSet);
			MutablePBAttributeSet->SetHealth(PBAttributeSet->GetHealth() - 10.f);
		}
	}
}

void APBEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APBEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &APBEffectActor::OnOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &APBEffectActor::EndOverlap);
}


