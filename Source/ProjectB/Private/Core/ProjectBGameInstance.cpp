// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ProjectBGameInstance.h"
#include "Core/PBGameState.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "OnlineSessionSettings.h"
#include "SaveGame/SaveGamePlayerInfo.h"

const static FName SESSION_NAME = TEXT("My Session Game");

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

#if !WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("IS NOT EDITOR"));
#else 
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("IS EDITOR"));
#endif
}

void UProjectBGameInstance::TryHostGame()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else 
	{
		CreateSession();
	}
}

void UProjectBGameInstance::TryFindSessions(FString GameSessionId)
{
	GameSessionIdToJoin = GameSessionId;
	FindSessions();
}

void UProjectBGameInstance::CreateSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
#if WITH_EDITOR
	SessionSettings.bIsLANMatch = true;
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("CREATING LAN MATCH"));
#else
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("CREATING ONLINE MATCH"));
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
#endif

	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bShouldAdvertise = true;

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UProjectBGameInstance::OnCreateSession(FName SessionName, bool bSuccess)
{
	if (!bSuccess)
	{
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
		return;
	}

	CreateSession();
}

void UProjectBGameInstance::StartHostingGame()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FString LobbyPath = LobbyLevel.GetLongPackageName();
	World->ServerTravel(LobbyPath + "?listen");
}

void UProjectBGameInstance::FindSessions()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!SessionSearch.IsValid())
	{
		return;
	}

#if WITH_EDITOR
	SessionSearch->bIsLanQuery = true;
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("FINDING LAN MATCH"));
#else
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("FINDING ONLINE MATCH"));
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
#endif
	SessionSearch->MaxSearchResults = 1000;

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UProjectBGameInstance::OnSessionsFound(bool bSuccess)
{
	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Check sessions found"));
	if (!bSuccess)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Could not find sessions"));
		GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Could not find sessions"));
		return;
	}
	if (!SessionSearch.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("SessionSearchNotValid"));
		return;
	}
	
	int FoundSessionIndex = 0;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (SearchResult.GetSessionIdStr() == GameSessionIdToJoin)
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining"));
			break;
		}
		GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, (TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr()));
		//UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
		FoundSessionIndex++;
	}

	if (FoundSessionIndex < SessionSearch->SearchResults.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("Try to JOIN"));
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[FoundSessionIndex]);
	}
}

void UProjectBGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FString Address;
	SessionInterface->GetResolvedConnectString(SessionName, Address);

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("JOINING"));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

FString UProjectBGameInstance::GetCurrentSessionID()
{
	if (!SessionInterface.IsValid())
	{
		return "NoSessionInterfaceFound";
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (!ExistingSession)
	{
		return "NoCurrentSessionFound";
	}

	return ExistingSession->GetSessionIdStr();;
}

void UProjectBGameInstance::SetConnectedPlayersCount()
{
	ConnectedPlayersCount = GetWorld()->GetGameState()->PlayerArray.Num();
}

void UProjectBGameInstance::SetInitialSaveGame()
{
	//PBTODO: create my own save game

	if (!PlayersInfoSaveGameClass)
		return;
	USaveGamePlayerInfo* TestSaveGame = Cast<USaveGamePlayerInfo>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayerInfo::StaticClass()));
	UGameplayStatics::SaveGameToSlot(TestSaveGame, TEXT("TestSlot"), 0);
	TESTInitialSaveGame();
}