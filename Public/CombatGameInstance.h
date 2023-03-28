// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "CombatGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCombatCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FCombatCharacterData() : Level(1), MaxHP(500.f), MaxStamina(50.f), Attack(10.f), MaxExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
		int32 MaxExp;
};

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEnemyData() : Name("Squire"), Level(1), MaxHP(200.f), Attack(30.0f), DropExp(10) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;
};

UCLASS()
class COMBAT_API UCombatGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCombatGameInstance();

	virtual void Init() override;

public:
	FCombatCharacterData* GetCombatCharacterData(int32 Level);

	FEnemyData* GetEnemyData(int32 Row);

	FStreamableManager StreamableManager;

private:
	UPROPERTY()
		class UDataTable* CombatCharacterTable;

	UPROPERTY()
		class UDataTable* EnemyTable;
};

