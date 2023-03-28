// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatGameMode.h"
#include "InventoryHUD.h"
#include "CombatCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Combat.h"
#include "CombatPlayerController.h"
#include "CombatPlayerState.h"
#include "CombatHUDWidget.h"

void ACombatGameMode::BeginPlay()
{
	//ApplyHUDChanges();
}

ACombatGameMode::ACombatGameMode():Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	PlayerControllerClass = ACombatPlayerController::StaticClass();
	PlayerStateClass = ACombatPlayerState::StaticClass();

	HUDClass = AInventoryHUD::StaticClass();

	//HUDState = EHUDState::HS_InGame;
}

void ACombatGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto CombatPlayerState = Cast<ACombatPlayerState>(NewPlayer->PlayerState);
	PCHECK(nullptr != CombatPlayerState);
	CombatPlayerState->InitPlayerData();
}

void ACombatGameMode::ApplyHUDChanges()
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	switch (HUDState)
	{
		case EHUDState::HS_InGame:
		{
			APlayerController* MyController = GetWorld()->GetFirstPlayerController();
			MyController->bShowMouseCursor = false;
			MyController->bEnableClickEvents = false;
			break;
		}
		case EHUDState::HS_Inventory:
		{
			ApplyHUD(InventoryHUDClass, true, true);
			break;
		}
		case EHUDState::HS_Equip:
		{
			ApplyHUD(EquipHUDClass, true, true);
			break;
		}
		case EHUDState::HS_Shop:
		{
			ApplyHUD(ShopHUDClass, true, true);
			break;
		}
		default:
		{
			ApplyHUD(false, false, false);
			break;
		}
	}
}

uint8 ACombatGameMode::GetHUDState()
{
	return HUDState;
}

void ACombatGameMode::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool ACombatGameMode::ApplyHUD(TSubclassOf<class UCombatHUDWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents)
{
	ACombatCharacter* MyCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	if (WidgetToApply != nullptr)
	{
		MyController->bShowMouseCursor = bShowMouseCursor;
		MyController->bEnableClickEvents = EnableClickEvents;

		//ChangeInputMode(false);

		CurrentWidget = CreateWidget<UCombatHUDWidget>(GetWorld(), WidgetToApply);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		else return false;
	}
	else return false;
}

/*
void ACombatGameMode::ChangeInputMode(bool bGameMode)
{
	ACombatCharacter* MyCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	if (bGameMode)
	{
		MyController->SetInputMode(GameInputMode);
		MyController->bShowMouseCursor = false;
	}
	else
	{
		MyController->SetInputMode(UIInputMode);
		MyController->bShowMouseCursor = true;
	}
}
*/