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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLeftTheGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerJoinedTheGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerChangedTheGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNetworkFailure);

//PBTODO: Rename UProjectBGameInstance to UPBGameInstance

UCLASS()
class PBP_API UProjectBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UProjectBGameInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	UFUNCTION(BlueprintCallable)
	void LeaveGame();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LobbyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> MainMenuLevel;
	//Will only exist on the server
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ConnectedPlayersCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USaveGamePlayerInfo> PlayersInfoSaveGameClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SaveGameSlotName = "TestSlot";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAbilitiesConfig> AbilitiesDataAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bSaveGameAlreadyCreated = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsInTransition = false;

	UPROPERTY(BlueprintAssignable)
	FOnLoadingStateUpdated OnLoadingStateUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnOperationFailed OnOperationFailed;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerLeftTheGame OnPlayerLeftTheGameDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerJoinedTheGame OnPlayerJoinedTheGameDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerChangedTheGame OnPlayerChangedTheGameDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnNetworkFailure OnNetworkFailureDelegate;

private:
	UFUNCTION(BlueprintCallable)
	void ResetGameInstance();

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
	void OnPlayerLeftTheGame(FName SessionName, const FUniqueNetId& UserID, EOnSessionParticipantLeftReason LeaveReason);
	void OnPlayerJoinedTheGame(FName SessionName, const FUniqueNetId& UserID);
	void OnPlayerChangedTheGame(FName SessionName, const FUniqueNetId& UserID, bool bWasThisAJoin);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	UPROPERTY()
	FString GameSessionIdToJoin;
};
