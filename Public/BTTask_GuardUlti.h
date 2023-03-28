// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GuardUlti.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTask_GuardUlti : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GuardUlti();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;
};
