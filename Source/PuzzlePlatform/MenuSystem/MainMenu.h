// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnHost;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnJoin;	
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnJoinMenu;	

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnCancelJoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IpAddressField;



	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	IMenuInterface* MenuInterface;

};
