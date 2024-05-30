// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ProjectBGameInstance.h"
#include "Core/PBGameState.h"
#include "Core/PBPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "OnlineSessionSettings.h"
#include "SaveGame/SaveGamePlayerInfo.h"

const static FName SESSION_NAME = TEXT("ProjectBSession");
const static FName SERVER_SETTINGS = TEXT("ProjectBServer");

UProjectBGameInstance::UProjectBGameInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UProjectBGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());

	SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found Session Interface"));

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UProjectBGameInstance::OnCreateSession);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UProjectBGameInstance::OnDestroySession);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UProjectBGameInstance::OnSessionsFound);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UProjectBGameInstance::OnJoinSession);
	SessionInterface->OnSessionParticipantLeftDelegates.AddUObject(this, &UProjectBGameInstance::OnPlayerLeftTheGame);
	SessionInterface->OnSessionParticipantJoinedDelegates.AddUObject(this, &UProjectBGameInstance::OnPlayerJoinedTheGame);
	SessionInterface->OnSessionParticipantsChangeDelegates.AddUObject(this, &UProjectBGameInstance::OnPlayerChangedTheGame);
	
	if (GEngine != nullptr) 
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UProjectBGameInstance::OnNetworkFailure);
	}
#if !WITH_EDITOR
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("IS NOT EDITOR"));
#else 
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("IS EDITOR"));
#endif
}

void UProjectBGameInstance::TryHostGame()
{
	OnLoadingStateUpdate.Broadcast(TEXT("Creating Server"));

	if (!SessionInterface.IsValid())
	{
		OnOperationFailed.Broadcast();
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	
	CreateSession();
}

void UProjectBGameInstance::TryFindSessions(FString GameSessionId)
{
	GameSessionIdToJoin = GameSessionId;
	FindSessions();
}

void UProjectBGameInstance::ResetGameInstance()
{
	bSaveGameAlreadyCreated = false;
	ConnectedPlayersCount = 0;
}

void UProjectBGameInstance::CreateSession()
{
	if (!SessionInterface.IsValid())
	{
		OnOperationFailed.Broadcast();
		return;
	}

	FOnlineSessionSettings SessionSettings;
#if WITH_EDITOR
	SessionSettings.bIsLANMatch = true;
	OnLoadingStateUpdate.Broadcast(TEXT("CREATING LAN MATCH"));
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("CREATING LAN MATCH"));
#else
	OnLoadingStateUpdate.Broadcast(TEXT("CREATING ONLINE MATCH"));
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("CREATING ONLINE MATCH"));
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
#endif

	FString ServerName = "ProjectBServerName";
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.Set(SERVER_SETTINGS, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UProjectBGameInstance::OnCreateSession(FName SessionName, bool bSuccess)
{
	if (!bSuccess)
	{
		OnOperationFailed.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	
	StartHostingGame();
}

void UProjectBGameInstance::OnDestroySession(FName SessionName, bool bSuccess)
{
	if(!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not destroy session"));
		OnOperationFailed.Broadcast();
		return;
	}
}

void UProjectBGameInstance::StartHostingGame()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	//Clear PlayersInfoSaveGame
	UGameplayStatics::DeleteGameInSlot(SaveGameSlotName, 0);

	FString LobbyPath = LobbyLevel.GetLongPackageName();
	World->ServerTravel(LobbyPath + "?listen");
}

void UProjectBGameInstance::FindSessions()
{
	if (!SessionInterface.IsValid())
	{
		OnOperationFailed.Broadcast();
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!SessionSearch.IsValid())
	{
		OnOperationFailed.Broadcast();
		return;
	}

	SessionSearch->MaxSearchResults = 1000;

#if WITH_EDITOR
	SessionSearch->bIsLanQuery = true;
	OnLoadingStateUpdate.Broadcast(TEXT("FINDING LAN MATCH"));
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("FINDING LAN MATCH"));
#else
	SessionSearch->bIsLanQuery = false;
	OnLoadingStateUpdate.Broadcast(TEXT("FINDING ONLINE MATCH"));
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("FINDING ONLINE MATCH"));
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->MaxSearchResults = 1000;
#endif
	
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UProjectBGameInstance::OnSessionsFound(bool bSuccess)
{
	OnLoadingStateUpdate.Broadcast(TEXT("Check sessions found"));
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Check sessions found"));
	if (!bSuccess)
	{
		OnOperationFailed.Broadcast();
		OnLoadingStateUpdate.Broadcast(TEXT("Could not find sessions"));
		//UE_LOG(LogTemp, Warning, TEXT("Could not find sessions"));
		//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Could not find sessions"));
		return;
	}
	if (!SessionSearch.IsValid())
	{
		OnOperationFailed.Broadcast();
		OnLoadingStateUpdate.Broadcast(TEXT("SessionSearch Not Valid"));
		//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("SessionSearchNotValid"));
		return;
	}

	int FoundSessionIndex = 0;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (SearchResult.GetSessionIdStr() == GameSessionIdToJoin)
		{
			OnLoadingStateUpdate.Broadcast(TEXT("Joining"));
			UE_LOG(LogTemp, Warning, TEXT("Joining"));
			break;
		}
		OnLoadingStateUpdate.Broadcast(TEXT("Found session"));
		//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, (TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr()));
		//UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
		FoundSessionIndex++;
	}

	if (FoundSessionIndex < SessionSearch->SearchResults.Num())
	{
		OnLoadingStateUpdate.Broadcast(TEXT("Try to join"));
		//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Try to join"));
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[FoundSessionIndex]);
	}
	else 
	{
		OnOperationFailed.Broadcast();
		OnLoadingStateUpdate.Broadcast(TEXT("No Sessions found"));
		//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("No Sessions found"));
		return;
	}
}

void UProjectBGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		OnOperationFailed.Broadcast();
		return;
	}

	FString Address;
	SessionInterface->GetResolvedConnectString(SessionName, Address);

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	OnLoadingStateUpdate.Broadcast(TEXT("JOINING"));
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("JOINING"));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute); 
}

void UProjectBGameInstance::OnPlayerLeftTheGame(FName SessionName, const FUniqueNetId& UserID, EOnSessionParticipantLeftReason LeaveReason)
{
	OnPlayerLeftTheGameDelegate.Broadcast();
}

void UProjectBGameInstance::OnPlayerJoinedTheGame(FName SessionName, const FUniqueNetId& UserID)
{
	OnPlayerJoinedTheGameDelegate.Broadcast();
}

void UProjectBGameInstance::OnPlayerChangedTheGame(FName SessionName, const FUniqueNetId& UserID, bool bWasThisAJoin)
{
	OnPlayerChangedTheGameDelegate.Broadcast();
}

void UProjectBGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	OnNetworkFailureDelegate.Broadcast();
}

FString UProjectBGameInstance::GetCurrentSessionID()
{
	if (!SessionInterface.IsValid())
	{
		OnOperationFailed.Broadcast();
		OnLoadingStateUpdate.Broadcast(TEXT("No SessionInterface Found"));
		return "NoSessionInterfaceFound";
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (!ExistingSession)
	{
		OnOperationFailed.Broadcast();
		OnLoadingStateUpdate.Broadcast(TEXT("No CurrentSession Found"));
		return "NoCurrentSessionFound";
	}

	return ExistingSession->GetSessionIdStr();
}

void UProjectBGameInstance::SetConnectedPlayersCount()
{
	ConnectedPlayersCount = GetWorld()->GetGameState()->PlayerArray.Num();
}

bool UProjectBGameInstance::SetInitialSaveGame()
{
	if (!PlayersInfoSaveGameClass || bSaveGameAlreadyCreated)
		return false;

	USaveGamePlayerInfo* TestSaveGame = Cast<USaveGamePlayerInfo>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayerInfo::StaticClass()));
	check(GetWorld()->GetGameState());
	
	for (APlayerState* Player : GetWorld()->GetGameState()->PlayerArray)
	{
		TestSaveGame->AddPlayerToInfo(Cast<APBPlayerState>(Player));
	}
	
	UGameplayStatics::SaveGameToSlot(TestSaveGame, SaveGameSlotName, 0);
	bSaveGameAlreadyCreated = true;
	return true;
}
USaveGamePlayerInfo* UProjectBGameInstance::GetPlayerInfoSaveGame()
{
	return Cast<USaveGamePlayerInfo>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
}

void UProjectBGameInstance::SavePlayersInfoSaveGame(USaveGamePlayerInfo* SaveGame)
{
	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGameSlotName, 0);
}

void UProjectBGameInstance::LeaveGame()
{	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FString MainMenuPath = MainMenuLevel.GetLongPackageName();
	PlayerController->ClientTravel(MainMenuPath, ETravelType::TRAVEL_Absolute);

	if (SessionInterface)
		SessionInterface->DestroySession(SESSION_NAME);
}
