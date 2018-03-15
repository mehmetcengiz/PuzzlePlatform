// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

/*Local files.*/
#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = TEXT("My Session Name");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != NULL)) { return; }
	MenuClass = MenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UInGameMenu> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != NULL)) { return; }
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init() {
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!ensure(OnlineSubsystem != NULL)) return;
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (SessionInterface.IsValid()) {
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
	}
}

void UPuzzlePlatformGameInstance::LoadMenuWidget() {
	if (!ensure(MenuClass != NULL)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != NULL)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);

}

void UPuzzlePlatformGameInstance::InGameLoadMenu() {
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::Host(FString ServerName) {
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession == nullptr) {
			CreateSession();
		}
		else {
			SessionInterface->DestroySession(SESSION_NAME);
		}

	}
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSetting;
		
		//Lan match depend on we use debugging or not.
		if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
			SessionSetting.bIsLANMatch = true;
		}else {
			SessionSetting.bIsLANMatch = false;
		}

		SessionSetting.NumPublicConnections = 2;
		SessionSetting.bShouldAdvertise = true;
		SessionSetting.bUsesPresence = true;
		SessionSetting.Set(SERVER_NAME_SETTINGS_KEY,DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSetting);
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {

	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session!"))
			return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");

}

void UPuzzlePlatformGameInstance::RefreshServerList() {

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		//SessionSearch->QuerySettings. //using for steam.
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}

}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success) {
	
	if (Success && SessionSearch.IsValid() && Menu != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Finished Finding Sessions:"));

		TArray<FServerData> ServerData;
		/*TODO Added for debugging. Delete 
		ServerNames.Add("Test Server 1");
		ServerNames.Add("Test Server 2");
		ServerNames.Add("Test Server 3");
		*/
		for (const FOnlineSessionSearchResult& search_result : SessionSearch->SearchResults) {
			UE_LOG(LogTemp, Warning, TEXT("Found Session Names: %s"), *search_result.GetSessionIdStr());
			FServerData Data;
			
			Data.MaxPlayers = search_result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - search_result.Session.NumOpenPublicConnections;
			Data.HostUsername = search_result.Session.OwningUserName;
			FString ServerName;
			if(search_result.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, OUT ServerName)) {
				Data.Name = ServerName;
			}else {
				Data.Name = "Could not find name.";
			}

			ServerData.Add(Data);
		}

		Menu->SetServerList(ServerData);
	}
}


void UPuzzlePlatformGameInstance::Join(uint32 Index) {

	if (!SessionInterface.IsValid()) {return;}
	if (!SessionSearch.IsValid()) { return; }

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) { return;}

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, OUT Address)) {
		UE_LOG(LogTemp, Error, TEXT("Could not get connect string!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController!= nullptr)) return;

	PlayerController->ClientTravel(Address, TRAVEL_Absolute);

}

void UPuzzlePlatformGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", TRAVEL_Absolute);
}