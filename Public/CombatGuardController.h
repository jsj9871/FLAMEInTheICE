// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatGuardController.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatGuardController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACombatGuardController();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

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
};
