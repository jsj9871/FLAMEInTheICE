// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerController.h"
#include "Combat.h"
#include "CombatPlayerState.h"
#include "CombatHUDWidget.h"
#include "CombatCharacter.h"
#include "CombatGameWidget.h"
#include "Blueprint/UserWidget.h"
#include "EnemyKnight.h"

ACombatPlayerController::ACombatPlayerController()
{
	static ConstructorHelpers::FClassFinder<UCombatHUDWidget> UI_HUD(TEXT("/Game/ThirdPerson/Blueprints/HUDs/HUD_InGame.HUD_InGame_C"));
	if (UI_HUD.Succeeded())
	{
		HUDWidgetClass = UI_HUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UCombatGameWidget> UI_MENU_C(TEXT("/Game/ThirdPerson/Blueprints/HUDs/BP_Pause.BP_Pause_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}
}

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	HUDWidget = CreateWidget<UCombatHUDWidget>(this, HUDWidgetClass);
	PCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	CombatPlayerState = Cast<ACombatPlayerState>(PlayerState);
	PCHECK(nullptr != CombatPlayerState);
	HUDWidget->BindPlayerState(CombatPlayerState);
	CombatPlayerState->OnPlayerStateChanged.Broadcast();
}

void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &ACombatPlayerController::Pause);
}

void ACombatPlayerController::Pause()
{
	MenuWidget = CreateWidget<UCombatGameWidget>(this, MenuWidgetClass);
	PCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

void ACombatPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACombatPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

UCombatHUDWidget* ACombatPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ACombatPlayerController::EnemyKill(AEnemyKnight * Killed) const
{
	//auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//PCHECK(nullptr != CombatCharacter);
	//CombatPlayerState = Cast<ACombatPlayerState>(PlayerState);
	//PCHECK(nullptr != CombatPlayerState);
	CombatPlayerState->AddExp(Killed->GetExp());
}

void ACombatPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}
