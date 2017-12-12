// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"


UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != NULL)) { return;}
	
	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"),*MenuClass->GetName());
}

void UPuzzlePlatformGameInstance::LoadMenu() {
	if (!ensure(MenuClass != NULL)) return;
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!ensure(Menu != NULL)) return;
	Menu->AddToViewport();

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
