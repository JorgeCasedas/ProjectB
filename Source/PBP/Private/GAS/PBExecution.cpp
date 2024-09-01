// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PBExecution.h"
#include "AbilitySystemComponent.h"
#include "GAS/PBHealthAttributeSet.h"
#include "Core/PBGameMode.h"
#include "Character/PBCharacter.h"
#include "Core/PBPlayerState.h"
#include "Kismet/GameplayStatics.h"

struct PBExecStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	PBExecStatics() 
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPBHealthAttributeSet, Health, Target, false);
	}
};

static const PBExecStatics& ExecStatics()
{
	static PBExecStatics Statics;
	return Statics;
}

UPBExecution::UPBExecution()
{
	RelevantAttributesToCapture.Add(ExecStatics().HealthDef);
}

void UPBExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	FGameModeSettings CurrentGameModeSettings = Cast<APBGameMode>(UGameplayStatics::GetGameMode(SourceAvatar))->CurrentGameModeSettings;
	bool bIsFriendlyFireActive = CurrentGameModeSettings.WinConditions.GameRules.Contains(EGameRule::FriendlyFire);
	bool bSameTeam = false;
	if (SourceASC != TargetASC && !bIsFriendlyFireActive)
	{
		if (const APBCharacter* SourceCharacter = Cast<APBCharacter>(SourceAvatar))
		{
			if (const APBCharacter* TargetCharacter = Cast<APBCharacter>(TargetAvatar))
			{
				if (const APBPlayerState* SourcePlayerState = Cast<APBPlayerState>(SourceCharacter->GetPlayerState()))
				{
					if (const APBPlayerState* TargetPlayerState = Cast<APBPlayerState>(TargetCharacter->GetPlayerState()))
					{
						int SourceTeamID = const_cast<APBPlayerState*>(SourcePlayerState)->GetTeamID();
						int TargetTeamID = const_cast<APBPlayerState*>(TargetPlayerState)->GetTeamID();
						
						if (SourceTeamID == TargetTeamID)
						{
							bSameTeam = true;
						}
					}
				}
			}	
		}
	}

	bool bShouldDealDamage = true;

	if (CurrentGameModeSettings.WinConditions.WinCondition == EWinCondition::PassTheBomb ||  
		CurrentGameModeSettings.WinConditions.WinCondition == EWinCondition::PushYourOpponentOutOfTheArena ||
		(!bIsFriendlyFireActive && bSameTeam))
	{
		bShouldDealDamage = false;
	}

	if(bShouldDealDamage || !Cast<APawn>(TargetAvatar))
	{
		float Health = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ExecStatics().HealthDef, EvaluationParameters, Health);
		FGameplayModifierEvaluatedData EvaluatedData(ExecStatics().HealthProperty, EGameplayModOp::Override, Health);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
	}

	
}
