// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"



bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(BtnHost != NULL)) return false;
	if (!ensure(BtnJoin != NULL)) return false;
	if (!ensure(BtnJoinMenu != NULL)) return false;
	if (!ensure(BtnCancelJoinMenu != NULL)) return false;

	BtnHost->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	BtnJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	BtnJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	BtnCancelJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;

}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}
void UMainMenu::Setup() {
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != NULL)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {
	this->RemoveFromViewport();

	if (!ensure(InWorld != NULL)) return;

	APlayerController* PlayerController = InWorld->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;

}

void UMainMenu::HostServer() {
	UE_LOG(LogTemp, Warning, TEXT("I will HOST a Server!!"));
	if(MenuInterface != NULL) {
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer() {
	UE_LOG(LogTemp, Warning, TEXT("I will Join to Server"));
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
