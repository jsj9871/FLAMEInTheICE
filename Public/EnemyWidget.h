// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindEnemyStat(class UEnemyStatComponent* NewEnemyStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UEnemyStatComponent> CurrentEnemyStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
