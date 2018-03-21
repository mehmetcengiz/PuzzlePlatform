// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "TimerManager.h"

#include "PuzzlePlatformGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if(NumberOfPlayers >=2 ) {
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 players!"));
		GetWorldTimerManager().SetTimer(OUT GameStartTimer, this, &ALobbyGameMode::StartGame, LobbyWaitingSeconds);
	}
}

void ALobbyGameMode::Logout(AController* Existing) {
	Super::Logout(Existing);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame() {
	auto GameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");

}
