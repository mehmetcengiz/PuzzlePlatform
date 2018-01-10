// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

/*Local files.*/
#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = TEXT("My Session Name");

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

void UPuzzlePlatformGameInstance::Host() {
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
		SessionSetting.bIsLANMatch = true;
		SessionSetting.NumPublicConnections = 2;
		SessionSetting.bShouldAdvertise = true;
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

	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");

}

void UPuzzlePlatformGameInstance::RefreshServerList() {

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		SessionSearch->bIsLanQuery = true;
		//SessionSearch->QuerySettings. //using for steam.
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}

}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success) {
	UE_LOG(LogTemp, Warning, TEXT("Finished Finding Sessions:"));
	if (Success && SessionSearch.IsValid() && Menu != NULL) {

		TArray<FString> ServerNames;

		for (const FOnlineSessionSearchResult& search_result : SessionSearch->SearchResults) {
			UE_LOG(LogTemp, Warning, TEXT("Found Session Names: %s"), *search_result.GetSessionIdStr());
			ServerNames.Add(*search_result.GetSessionIdStr());
		}

		Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Address) {

	if (Menu != NULL) {
		//Menu->SetServerList({ "Test1", "Test2", "Test3"});
	}


	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	//APlayerController* PlayerController = GetFirstLocalPlayerController();
	//if (!ensure(PlayerController!= nullptr)) return;

	//PlayerController->ClientTravel(Address, TRAVEL_Absolute);

}

void UPuzzlePlatformGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", TRAVEL_Absolute);
}