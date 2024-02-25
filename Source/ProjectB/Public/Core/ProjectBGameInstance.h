// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "ProjectBGameInstance.generated.h"


class FOnlineSessionSearch;
class USaveGamePlayerInfo;
class UAbilitiesConfig;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingStateUpdated, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOperationFailed);

//PBTODO: Rename UProjectBGameInstance to UPBGameInstance

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
	UFUNCTION(BlueprintCallable)
	void SetConnectedPlayersCount();

	UFUNCTION(BlueprintCallable)
	bool SetInitialSaveGame();
	UFUNCTION(BlueprintCallable)
	USaveGamePlayerInfo* GetPlayerInfoSaveGame();
	UFUNCTION(BlueprintCallable)
	void SavePlayersInfoSaveGame(USaveGamePlayerInfo* SaveGame);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LobbyLevel;
	//Will only exist on the server
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ConnectedPlayersCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USaveGamePlayerInfo> PlayersInfoSaveGameClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SaveGameSlotName = "TestSlot";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAbilitiesConfig> AbilitiesDataAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSaveGameAlreadyCreated = false;

	UPROPERTY(BlueprintAssignable)
	FOnLoadingStateUpdated OnLoadingStateUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnOperationFailed OnOperationFailed;

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
	UPROPERTY()
	FString GameSessionIdToJoin;
};
