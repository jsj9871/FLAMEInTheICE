// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;	// 공격 상태
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
		EnemyKnight->Attack();	// 공격 시
		IsAttacking = true;	// 공격 중
	}
	else if (EnemyKnight->Name == "Veneret")
	{
		EnemyKnight->LongAttack();	// 공격 시
		IsAttacking = true;	// 공격 중
	}
	else if (EnemyKnight->Name == "Baron")
	{
		
		if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 1000.f))
		{
			EnemyKnight->LongAttack();	// 공격 시
		}
		else if ((Target->GetDistanceTo(ControllingPawn) <= 200.f))
		{
			EnemyKnight->Attack();	// 공격 시
		}

		IsAttacking = true;	// 공격 중
	}
	else if (EnemyKnight->Name == "Count")
	{
		if ((Target->GetDistanceTo(ControllingPawn) > 500.f) && (Target->GetDistanceTo(ControllingPawn) <= 1000.f))
		{
			//EnemyKnight->CountUlti();
			EnemyKnight->LongAttack();	// 공격 시
		}
		else if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 500.f))
		{
			EnemyKnight->CountUlti();
		}
		else if ((Target->GetDistanceTo(ControllingPawn) <= 200.f))
		{
			EnemyKnight->Attack();	// 공격 시
		}

		IsAttacking = true;	// 공격 중
	}
	else if (EnemyKnight->Name == "Guard")
	{
		if ((Target->GetDistanceTo(ControllingPawn) > 400.f) && (Target->GetDistanceTo(ControllingPawn) <= 1000.f))
		{
			EnemyKnight->LongAttack();	// 공격 시
		}
		else if ((Target->GetDistanceTo(ControllingPawn) > 200.f) && (Target->GetDistanceTo(ControllingPawn) <= 400.f))
		{
			EnemyKnight->GuardUlti();
		}
		else if ((Target->GetDistanceTo(ControllingPawn) <= 200.f))
		{
			EnemyKnight->Attack();	// 공격 시
		}

		IsAttacking = true;	// 공격 중
	}

	EnemyKnight->OnAttackEnd.AddLambda([this]() -> void {	// 공격 종료 시
		IsAttacking = false;	// 공격 중 X
		});

	return EBTNodeResult::InProgress;
}

// 매 프레임 실행
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)	// 공격 중 X
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
