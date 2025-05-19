// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBGameplayAbilityWithCharges.h"
#include "Core/PBPlayerController.h"
#include "Core/ProjectBGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UPBGameplayAbilityWithCharges::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	Cast<APBPlayerController>(ActorInfo->PlayerController)->OnAbilityAdded(this);
}

void UPBGameplayAbilityWithCharges::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	Cast<APBPlayerController>(ActorInfo->PlayerController)->OnAbilityRemoved(this);
}

bool UPBGameplayAbilityWithCharges::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	UProjectBGameInstance* GameInstance = Cast<UProjectBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance != nullptr)
	{
		float AbilityCharges = GameInstance->GetChargeAttributeValueByCooldownTag(CdTag);

		return AbilityCharges > 0;
	}

	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}
