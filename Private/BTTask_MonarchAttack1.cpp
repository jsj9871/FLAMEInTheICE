// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonarchAttack1.h"
#include "CombatCharacter.h"
#include "CombatBossController.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MonarchAttack1::UBTTask_MonarchAttack1()
{
	bNotifyTick = true;
	IsAttacking = false;	// ���� ����
}

EBTNodeResult::Type UBTTask_MonarchAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatBossController::TargetKey));

	auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EnemyKnight)
		return EBTNodeResult::Failed;

	if ((Target->GetDistanceTo(ControllingPawn) <= 250.f ))
	{
		EnemyKnight->Attack();	// ���� ��
	}
	else if ((Target->GetDistanceTo(ControllingPawn) > 250.f))
	{
		EnemyKnight->MonarchBurst();	// ���� ��
	}

	IsAttacking = true;	// ���� ��

	EnemyKnight->OnAttackEnd.AddLambda([this]() -> void {	// ���� ���� ��
		IsAttacking = false;	// ���� �� X
		});

	return EBTNodeResult::Type();
}

void UBTTask_MonarchAttack1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)	// ���� �� X
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


