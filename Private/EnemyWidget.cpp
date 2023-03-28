// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "EnemyStatComponent.h"
#include "Components/ProgressBar.h"
#include "Combat.h"

void UEnemyWidget::BindEnemyStat(UEnemyStatComponent* NewEnemyStat)
{
	PCHECK(nullptr != NewEnemyStat);

	CurrentEnemyStat = NewEnemyStat;

	NewEnemyStat->OnHPChanged.AddUObject(this, &UEnemyWidget::UpdateHPWidget);
}

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	PCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UEnemyWidget::UpdateHPWidget()
{
	if (CurrentEnemyStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentEnemyStat->GetHPRatio());
		}
	}
}
