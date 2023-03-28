// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CombatSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCombatSaveGame();

	UPROPERTY()
		int32 Level;

	UPROPERTY()
		int32 CurrentExp;

	UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		int32 CharacterIndex;
};
