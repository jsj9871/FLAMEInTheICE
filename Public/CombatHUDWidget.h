// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void BindCharacterStat(class UCombatCharacterStatComponent* CharacterStat);
    void BindPlayerState(class ACombatPlayerState* PlayerState);
    void UpdateCharacterStat();

    void EquipVisibility();

protected:
    virtual void NativeConstruct() override;
    void UpdatePlayerState();

private:
    TWeakObjectPtr<class UCombatCharacterStatComponent> CurrentCharacterStat;
    TWeakObjectPtr<class ACombatPlayerState> CurrentPlayerState;

    UPROPERTY()
        class UProgressBar* HPBar;

    UPROPERTY()
        class UProgressBar* StaminaBar;

    UPROPERTY()
        class UProgressBar* ExpBar;

    UPROPERTY()
        class UTextBlock* PlayerName;

    UPROPERTY()
        class UTextBlock* PlayerLevel;

public:
    UPROPERTY()
        class UTextBlock* Equip;
};
