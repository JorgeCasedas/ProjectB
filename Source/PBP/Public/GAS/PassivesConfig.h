#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PassivesConfig.generated.h"

class UGameplayEffect;
/**
 *
 */

USTRUCT(BlueprintType)
struct FPBPassiveInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Info;

};

UCLASS()
class PBP_API UPassivesConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPBPassiveInfo> EffectsInfo;
};