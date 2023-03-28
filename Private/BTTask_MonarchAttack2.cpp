// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonarchAttack2.h"
#include "CombatCharacter.h"
#include "CombatBossController.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MonarchAttack2::UBTTask_MonarchAttack2()
{
	bNotifyTick = true;
	IsAttacking = false;	// ���� ����
}

EBTNodeResult::Type UBTTask_MonarchAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatBossController::TargetKey));

	auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EnemyKnight)
		return EBTNodeResult::Failed;

	if ((Target->GetDistanceTo(ControllingPawn) <= 250.f))
	{
		EnemyKnight->Attack();	// ���� ��
	}
	else if ((Target->GetDistanceTo(ControllingPawn) > 250.f))
	{
		EnemyKnight->MonarchTornado();	// ���� ��
	}

	IsAttacking = true;	// ���� ��

	EnemyKnight->OnAttackEnd.AddLambda([this]() -> void {	// ���� ���� ��
		IsAttacking = false;	// ���� �� X
		});


	return EBTNodeResult::Type();
}

void UBTTask_MonarchAttack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)	// ���� �� X
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
