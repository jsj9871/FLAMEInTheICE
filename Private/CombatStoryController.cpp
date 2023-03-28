// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStoryController.h"
#include "Blueprint/UserWidget.h"
#include "Combat.h"

void ACombatStoryController::BeginPlay()
{
	Super::BeginPlay();

	PCHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);

	PCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
