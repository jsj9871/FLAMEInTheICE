// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHUDWidget.h"
#include "Combat.h"
#include "EnemyStatComponent.h"
#include "Components/ProgressBar.h"

void UEnemyBossHUDWidget::BindEnemyStat(UEnemyStatComponent* EnemyStat)
{
    PCHECK(nullptr != EnemyStat);
    //CurrentEnemyStat = EnemyStat;
    EnemyStat->OnHPChanged.AddUObject(this, &UEnemyBossHUDWidget::UpdateEnemyStat);
}

void UEnemyBossHUDWidget::UpdateEnemyStat()
{
    PCHECK(CurrentEnemyStat.IsValid());

    MonarchHPBar->SetPercent(CurrentEnemyStat->GetHPRatio());
}

void UEnemyBossHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    MonarchHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_MonarchHPBar")));
    PCHECK(nullptr != MonarchHPBar);
}
