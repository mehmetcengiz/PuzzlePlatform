// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
		UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

		void Setup(class UMainMenu* ParentToSet, uint32 IndexToSet);
	
private:
		UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

		UPROPERTY()
		UMainMenu* Parent = nullptr;
		
		uint32 Index;

		UFUNCTION()
		void OnRowButtonClicked();



};
