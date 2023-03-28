// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatGameWidget.h"
#include "Components/Button.h"
#include "CombatPlayerController.h"

void UCombatGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnBack")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UCombatGameWidget::ResumeClicked);
	}

	ToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnMainMenu")));
	if (nullptr != ToTitleButton)
	{
		ToTitleButton->OnClicked.AddDynamic(this, &UCombatGameWidget::ToTitleClicked);
	}

	RetryButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UCombatGameWidget::RetryClicked);
	}
}

void UCombatGameWidget::ResumeClicked()
{
	auto CombatPlayerController = Cast<ACombatPlayerController>(GetOwningPlayer());
	PCHECK(nullptr != CombatPlayerController);

	RemoveFromParent();
	CombatPlayerController->ChangeInputMode(true);
	CombatPlayerController->SetPause(false);
}

void UCombatGameWidget::ToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UCombatGameWidget::RetryClicked()
{
	auto CombatPlayerController = Cast<ACombatPlayerController>(GetOwningPlayer());
	PCHECK(nullptr != CombatPlayerController);
	CombatPlayerController->RestartLevel();
}
