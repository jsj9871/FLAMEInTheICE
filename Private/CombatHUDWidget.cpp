// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatHUDWidget.h"
#include "Combat.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CombatCharacterStatComponent.h"
#include "CombatPlayerState.h"


// �÷��̾� HP, Stamina ���ε�
void UCombatHUDWidget::BindCharacterStat(UCombatCharacterStatComponent* CharacterStat)
{
    PCHECK(nullptr != CharacterStat);
    CurrentCharacterStat = CharacterStat;
    CharacterStat->OnHPChanged.AddUObject(this, &UCombatHUDWidget::UpdateCharacterStat);
    CharacterStat->OnStaminaChanged.AddUObject(this, &UCombatHUDWidget::UpdateCharacterStat);
}

// �÷��̾� Name, Level, Exp ���ε�
void UCombatHUDWidget::BindPlayerState(ACombatPlayerState* PlayerState)
{
    PCHECK(nullptr != PlayerState);
    CurrentPlayerState = PlayerState;
    PlayerState->OnPlayerStateChanged.AddUObject(this, &UCombatHUDWidget::UpdatePlayerState);
}

// ���ε� �ؾ��� �͵� ����
void UCombatHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
    PCHECK(nullptr != HPBar);

    StaminaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_StaminaBar")));
    PCHECK(nullptr != StaminaBar);

    ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_ExpBar")));
    PCHECK(nullptr != ExpBar);

    PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Name")));
    PCHECK(nullptr != PlayerName);

    PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Level")));
    PCHECK(nullptr != PlayerLevel);

    Equip = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Equip")));
    PCHECK(nullptr != Equip);
}

// �÷��̾� HP, Stamina ���� ����
void UCombatHUDWidget::UpdateCharacterStat()
{
    PCHECK(CurrentCharacterStat.IsValid());

    HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
    StaminaBar->SetPercent(CurrentCharacterStat->GetStaminaRatio());
}

void UCombatHUDWidget::EquipVisibility()
{
    
}

// �÷��̾� Name, Level, Exp ���� ����
void UCombatHUDWidget::UpdatePlayerState()
{
    PCHECK(CurrentPlayerState.IsValid());

    PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
    PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
    ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
}