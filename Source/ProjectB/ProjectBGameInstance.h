// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "ProjectBGameInstance.generated.h"


class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class PROJECTB_API UProjectBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UProjectBGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void TryHostGame();
	UFUNCTION(BlueprintCallable)
	void TryFindSessions(FString GameSessionId);
	UFUNCTION(BlueprintCallable)
	FString GetCurrentSessionID();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LobbyLevel;

private:
	UFUNCTION(BlueprintCallable)
	void CreateSession();
	UFUNCTION()
	void OnCreateSession(FName SessionName, bool bSuccess);
	UFUNCTION()
	void OnDestroySession(FName SessionName, bool bSuccess);

	UFUNCTION()
	void StartHostingGame();

	UFUNCTION()
	void FindSessions();
	UFUNCTION()
	void OnSessionsFound(bool bSuccess);

	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FString GameSessionIdToJoin;
};
