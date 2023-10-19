// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ProjectBEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/ProjectBAttributeSet.h"
#include "Components/CapsuleComponent.h"

AProjectBEffectActor::AProjectBEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
}

void AProjectBEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: make it work with GAS
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UProjectBAttributeSet* ProjectBAttributeSet = Cast<UProjectBAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UProjectBAttributeSet::StaticClass()));

		if (ProjectBAttributeSet)
		{
			//TODO: This is hack, so big NONO, change it as soon as possible
			UProjectBAttributeSet* MutableProjectBAttributeSet = const_cast<UProjectBAttributeSet*>(ProjectBAttributeSet);
			MutableProjectBAttributeSet->SetHealth(ProjectBAttributeSet->GetHealth() - 10.f);
		}
	}
}

void AProjectBEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AProjectBEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AProjectBEffectActor::OnOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AProjectBEffectActor::EndOverlap);
}


