// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonarchUlti.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MonarchUlti::UBTTask_MonarchUlti()
{
	bNotifyTick = true;
	IsAttacking = false;	// ���� ����
}

EBTNodeResult::Type UBTTask_MonarchUlti::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatBossController::TargetKey));

	auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EnemyKnight)
		return EBTNodeResult::Failed;

	else if ((Target->GetDistanceTo(ControllingPawn) > 250.f) || (Target->GetDistanceTo(ControllingPawn) <= 400.f))
	{
		EnemyKnight->MonarchUlti();	// ���� ��
	}

	IsAttacking = true;	// ���� ��

	EnemyKnight->OnAttackEnd.AddLambda([this]() -> void {	// ���� ���� ��
		IsAttacking = false;	// ���� �� X
		});

	return EBTNodeResult::Type();
}

void UBTTask_MonarchUlti::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)	// ���� �� X
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
