// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Regen.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTService_Regen : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Regen();

	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	void RegenHP();

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UEnemyStatComponent* EnemyCurrentStat;

	struct FEnemyData* EnemyStatData = nullptr;

private:
	FTimerHandle RegenHPHandle;
};
