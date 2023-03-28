// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnemyKnight.h"
#include "CombatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACombatPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UCombatHUDWidget* GetHUDWidget() const;

	void EnemyKill(AEnemyKnight* Killed) const;

	void ChangeInputMode(bool bGameMode = true);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UCombatHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UCombatGameWidget> MenuWidgetClass;

private:
	void Pause();

	UPROPERTY()
		class UCombatHUDWidget* HUDWidget;

	UPROPERTY()
		class ACombatPlayerState* CombatPlayerState;

	UPROPERTY()
		class UCombatGameWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
