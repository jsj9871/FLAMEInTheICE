// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatStoryController.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatStoryController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, Meta = (bIsFocusable = true))
		TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
		class UUserWidget* UIWidgetInstance;
};
