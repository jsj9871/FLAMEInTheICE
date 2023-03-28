// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAIController.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACombatAIController();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void RunAI();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopAI();

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

/*private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;*/
};
