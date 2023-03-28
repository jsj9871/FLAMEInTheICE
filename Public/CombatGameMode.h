// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGameMode.generated.h"

UCLASS(minimalapi)
class ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

public:
	ACombatGameMode();

	void PostLogin(APlayerController* NewPlayer);

	enum EHUDState : uint8
	{
		HS_InGame,
		HS_Inventory,
		HS_Equip,
		HS_Shop
	};
	
	void ApplyHUDChanges();

	uint8 GetHUDState();

	UFUNCTION(BluePrintCallable, Category = "HUD")
	void ChangeHUDState(uint8 NewState);

	bool ApplyHUD(TSubclassOf<class UCombatHUDWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents);

	void ChangeInputMode(bool bGameMode = true);
	
protected:
	uint8 HUDState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UCombatHUDWidget> InGameHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UCombatHUDWidget> InventoryHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UCombatHUDWidget> EquipHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UCombatHUDWidget> ShopHUDClass;

	UPROPERTY()
		class UCombatHUDWidget* CurrentWidget;

	//FInputModeGameOnly GameInputMode;
	//FInputModeUIOnly UIInputMode;
};



