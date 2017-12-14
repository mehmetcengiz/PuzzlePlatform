// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"


UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UMainMenu> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != NULL)) { return;}
	
	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"),*MenuClass->GetName());
}

void UPuzzlePlatformGameInstance::LoadMenu() {
	if (!ensure(MenuClass != NULL)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != NULL)) return;
	Menu->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;

	Menu->SetMenuInterface(this);

}

void UPuzzlePlatformGameInstance::Host() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformGameInstance::Join(const FString& Address) {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Joining %s"),*Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController!= nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

}
