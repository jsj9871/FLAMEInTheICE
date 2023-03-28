// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;	// ���� ����
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatAIController::TargetKey));

	auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EnemyKnight)
		return EBTNodeResult::Failed;
	
	if (EnemyKnight->Name == "Squire" || EnemyKnight->Name == "Peong" || EnemyKnight->Name == "Castellan")
	{
		EnemyKnight->Attack();	// ���� ��
		IsAttacking = true;	// ���� ��
	}
	else if (EnemyKnight->Name == "Veneret")
	{
		EnemyKnight->LongAttack();	// ���� ��
		IsAttacking = true;	// ���� ��
	}
	else if (EnemyKnight->Name == "Baron")
	{
		
		if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 1000.f))
		{
			EnemyKnight->LongAttack();	// ���� ��
		}
		else if ((Target->GetDistanceTo(ControllingPawn) <= 200.f))
		{
			EnemyKnight->Attack();	// ���� ��
		}

		IsAttacking = true;	// ���� ��
	}
	else if (EnemyKnight->Name == "Count")
	{
		if ((Target->GetDistanceTo(ControllingPawn) > 500.f) && (Target->GetDistanceTo(ControllingPawn) <= 1000.f))
		{
			//EnemyKnight->CountUlti();
			EnemyKnight->LongAttack();	// ���� ��
		}
		else if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 500.f))
		{
			EnemyKnight->CountUlti();
		}
		else if ((Target->GetDistanceTo(ControllingPawn) <= 200.f))
		{
			EnemyKnight->Attack();	// ���� ��
		}

		IsAttacking = true;	// ���� ��
	}
	else if (EnemyKnight->Name == "Guard")
	{
		if ((Target->GetDistanceTo(ControllingPawn) > 400.f) && (Target->GetDistanceTo(ControllingPawn) <= 1000.f))
		{
			EnemyKnight->LongAttack();	// ���� ��
		}
		else if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 400.f))
		{
			EnemyKnight->GuardUlti();
		}
		else if ((Target->GetDistanceTo(ControllingPawn) <= 200.f))
		{
			EnemyKnight->Attack();	// ���� ��
		}

		IsAttacking = true;	// ���� ��
	}

	EnemyKnight->OnAttackEnd.AddLambda([this]() -> void {	// ���� ���� ��
		IsAttacking = false;	// ���� �� X
		});

	return EBTNodeResult::InProgress;
}

// �� ������ ����
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)	// ���� �� X
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
