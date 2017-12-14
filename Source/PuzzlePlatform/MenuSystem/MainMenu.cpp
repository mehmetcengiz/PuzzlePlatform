// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(BtnHost != NULL)) return false;
	if (!ensure(BtnJoin != NULL)) return false;

	BtnHost->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	BtnJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;

}

void UMainMenu::HostServer() {
	UE_LOG(LogTemp, Warning, TEXT("I will HOST a Server!!"));
}

void UMainMenu::JoinServer() {
	UE_LOG(LogTemp, Warning, TEXT("I will Join to Server"));
}
