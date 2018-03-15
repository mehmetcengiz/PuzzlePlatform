// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if(NumberOfPlayers >=3 ) {
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 players!"));	
		
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	}
}

void ALobbyGameMode::Logout(AController* Existing) {
	Super::Logout(Existing);
	--NumberOfPlayers;
}
