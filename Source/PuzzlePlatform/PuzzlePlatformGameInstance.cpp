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


UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UMainMenu> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
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
	}
}

void UPuzzlePlatformGameInstance::LoadMenuWidget() {
	if (!ensure(MenuClass != NULL)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
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
		FOnlineSessionSettings SessionSetting;
		SessionInterface->CreateSession(0, TEXT("My Session Game"), SessionSetting);
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Address) {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController!= nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

}

void UPuzzlePlatformGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {

	if(!Success) {
		UE_LOG(LogTemp,Warning,TEXT("Could not create session!"))
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");

}
