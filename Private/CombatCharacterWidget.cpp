// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCharacterWidget.h"
#include "CombatCharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Combat.h"

void UCombatCharacterWidget::BindCharacterStat(UCombatCharacterStatComponent* NewCharacterStat)
{
	PCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;

	NewCharacterStat->OnHPChanged.AddUObject(this, &UCombatCharacterWidget::UpdateHPWidget);
	//NewCharacterStat->OnStaminaChanged.AddUObject(this, &UCombatCharacterWidget::UpdateStaminaWidget);
}

void UCombatCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	PCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();

	/*
	StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_StaminaBar")));
	PCHECK(nullptr != StaminaProgressBar);
	UpdateStaminaWidget();
	*/
}

// HP 바 업데이트
void UCombatCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}

// 스테미나 바 업데이트
void UCombatCharacterWidget::UpdateStaminaWidget()
{
	/*
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != StaminaProgressBar)
		{
			StaminaProgressBar->SetPercent(CurrentCharacterStat->GetStaminaRatio());
		}
	}
	*/
}
