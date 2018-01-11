// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UServerRow::Setup(UMainMenu * ParentToSet, uint32 IndexToSet)
{
	Parent = ParentToSet;
	Index = IndexToSet;

	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnRowButtonClicked);
}

void UServerRow::OnRowButtonClicked()
{
	Parent->SelectIndex(Index);
}
