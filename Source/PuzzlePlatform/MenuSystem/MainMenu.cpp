// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"



bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(BtnHost != NULL)) return false;
	if (!ensure(BtnJoin != NULL)) return false;
	if (!ensure(BtnJoinMenu != NULL)) return false;
	if (!ensure(BtnCancelJoinMenu != NULL)) return false;
	if (!ensure(IpAddressField != NULL)) return false;
	if (!ensure(BtnExitGame != NULL)) return false;

	BtnHost->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	BtnJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	BtnJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	BtnCancelJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	BtnExitGame->OnClicked.AddDynamic(this, &UMainMenu::ExitPressed);
	return true;

}


void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

}

void UMainMenu::HostServer() {
	UE_LOG(LogTemp, Warning, TEXT("I will HOST a Server!!"));
	if(MenuInterface != NULL) {
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer() {
	UE_LOG(LogTemp, Warning, TEXT("I will Join to Server"));
	if(MenuInterface != NULL) {
		if (!ensure(IpAddressField != NULL)) return;
		FString Address = IpAddressField->GetText().ToString();
		MenuInterface->Join(Address);
	}
}

void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != NULL)) return;
	if (!ensure(JoinMenu != NULL)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu() {
	if (!ensure(MenuSwitcher != NULL)) return;
	if (!ensure(MainMenu != NULL)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitPressed() {
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
