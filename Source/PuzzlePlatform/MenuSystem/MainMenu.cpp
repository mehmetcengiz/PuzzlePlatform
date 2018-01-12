// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(BtnHost != NULL)) return false;
	if (!ensure(BtnJoin != NULL)) return false;
	if (!ensure(BtnJoinMenu != NULL)) return false;
	if (!ensure(BtnCancelJoinMenu != NULL)) return false;
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

void UMainMenu::SetServerList(TArray<FString> ServerNames) {

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 index = 0;
	for (const FString& ServerName : ServerNames) {
		UServerRow * Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, index);
		++index;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index){
	SelectedIndex = Index;
}

void UMainMenu::JoinServer() {
	
	if (SelectedIndex.IsSet() && MenuInterface != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("You havent selected a index !!! "));
	}
		//if (!ensure(IpAddressField != NULL)) return;
		//FString Address = IpAddressField->GetText().ToString();
	
}

void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != NULL)) return;
	if (!ensure(JoinMenu != NULL)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr) { MenuInterface->RefreshServerList(); }
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
