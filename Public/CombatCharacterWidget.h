// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UCombatCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateStaminaWidget();

private:
	TWeakObjectPtr<class UCombatCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;

	//UPROPERTY()
		//class UProgressBar* StaminaProgressBar;
		
};
