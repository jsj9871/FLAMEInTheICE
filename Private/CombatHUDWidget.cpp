// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatHUDWidget.h"
#include "Combat.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CombatCharacterStatComponent.h"
#include "CombatPlayerState.h"


// 플레이어 HP, Stamina 바인딩
void UCombatHUDWidget::BindCharacterStat(UCombatCharacterStatComponent* CharacterStat)
{
    PCHECK(nullptr != CharacterStat);
    CurrentCharacterStat = CharacterStat;
    CharacterStat->OnHPChanged.AddUObject(this, &UCombatHUDWidget::UpdateCharacterStat);
    CharacterStat->OnStaminaChanged.AddUObject(this, &UCombatHUDWidget::UpdateCharacterStat);
}

// 플레이어 Name, Level, Exp 바인딩
void UCombatHUDWidget::BindPlayerState(ACombatPlayerState* PlayerState)
{
    PCHECK(nullptr != PlayerState);
    CurrentPlayerState = PlayerState;
    PlayerState->OnPlayerStateChanged.AddUObject(this, &UCombatHUDWidget::UpdatePlayerState);
}

// 바인딩 해야할 것들 연동
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

// 플레이어 HP, Stamina 비율 적용
void UCombatHUDWidget::UpdateCharacterStat()
{
    PCHECK(CurrentCharacterStat.IsValid());

    HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
    StaminaBar->SetPercent(CurrentCharacterStat->GetStaminaRatio());
}

void UCombatHUDWidget::EquipVisibility()
{
    
}

// 플레이어 Name, Level, Exp 비율 적용
void UCombatHUDWidget::UpdatePlayerState()
{
    PCHECK(CurrentPlayerState.IsValid());

    PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
    PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
    ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
}