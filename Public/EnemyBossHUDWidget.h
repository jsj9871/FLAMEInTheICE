// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBossHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyBossHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void BindEnemyStat(class UEnemyStatComponent* EnemyStat);
    void UpdateEnemyStat();

protected:
    virtual void NativeConstruct() override;

private:
    TWeakObjectPtr<class UEnemyStatComponent> CurrentEnemyStat;

    UPROPERTY()
        class UProgressBar* MonarchHPBar;
};
