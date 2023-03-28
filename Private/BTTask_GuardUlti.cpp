// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GuardUlti.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GuardUlti::UBTTask_GuardUlti()
{
	bNotifyTick = true;
	IsAttacking = false;	// ���� ����
}

EBTNodeResult::Type UBTTask_GuardUlti::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatAIController::TargetKey));

	auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EnemyKnight)
		return EBTNodeResult::Failed;

	else if (EnemyKnight->Name == "Guard")
	{
		if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 400.f))
		{
			EnemyKnight->GuardUlti();
		}
	}

	IsAttacking = true;	// ���� ��

	EnemyKnight->OnAttackEnd.AddLambda([this]() -> void {	// ���� ���� ��
		IsAttacking = false;	// ���� �� X
		});

	return EBTNodeResult::Type();
}

void UBTTask_GuardUlti::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)	// ���� �� X
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
