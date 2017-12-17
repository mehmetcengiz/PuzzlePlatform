// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"


bool UInGameMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!ensure(BtnResume != NULL)) return false;
	if (!ensure(BtnQuit != NULL)) return false;

	BtnResume->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
	BtnQuit->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}

void UInGameMenu::CancelPressed() {
	Teardown();
}

void UInGameMenu::QuitPressed() {
	if(MenuInterface != NULL) {
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
