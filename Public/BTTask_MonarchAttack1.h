// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MonarchAttack1.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTask_MonarchAttack1 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MonarchAttack1();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;
};
