// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbilityWithCharges.h"
#include "Core/PBPlayerController.h"
#include "Core/ProjectBGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UPBGameplayAbilityWithCharges::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if (!ActorInfo) //NULL ActorInfo is crashing the build sometimes when loading a new map
		return;
	if (APBPlayerController* PlayerController = Cast<APBPlayerController>(ActorInfo->PlayerController))
		PlayerController->OnAbilityAdded(this);
}

void UPBGameplayAbilityWithCharges::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	if (!ActorInfo)
		return;
	if (APBPlayerController* PlayerController = Cast<APBPlayerController>(ActorInfo->PlayerController))
		PlayerController->OnAbilityRemoved(this);
}

bool UPBGameplayAbilityWithCharges::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	UProjectBGameInstance* GameInstance = Cast<UProjectBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!ActorInfo)
		return false;
	if (GameInstance != nullptr)
	{
		float AbilityCharges = GameInstance->GetChargeAttributeValueByCooldownTag(*ActorInfo, CdTag);

		return AbilityCharges > 0;
	}

	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}
