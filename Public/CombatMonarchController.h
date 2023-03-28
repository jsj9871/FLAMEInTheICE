// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatMonarchController.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatMonarchController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACombatMonarchController();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	class UEnemyBossHUDWidget* GetHUDWidget() const;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

	UPROPERTY()
		class UEnemyBossHUDWidget* HUDWidget;
};
