// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void ResumeClicked();

	UFUNCTION()
		void ToTitleClicked();

	UFUNCTION()
		void RetryClicked();

protected:
	UPROPERTY()
		class UButton* ResumeButton;

	UPROPERTY()
		class UButton* ToTitleButton;

	UPROPERTY()
		class UButton* RetryButton;
};
