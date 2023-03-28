// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()   
{
    NodeName = TEXT("CanAttack");   // 노드 이름
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
        return false;

    // 공격 상대 지정
    auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatAIController::TargetKey));
    if (nullptr == Target)
        return false;

    auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == EnemyKnight)
        return EBTNodeResult::Failed;

    if (EnemyKnight->Name == "Squire" || EnemyKnight->Name == "Peong")
    {
        bResult = (Target->GetDistanceTo(ControllingPawn) <= 100.f);   // 공격 사거리
    }
    else if (EnemyKnight->Name == "Castellan")
    {
        bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.f);   // 공격 사거리
    }
    else if (EnemyKnight->Name == "Veneret" || EnemyKnight->Name == "Baron")
    {
        bResult = (Target->GetDistanceTo(ControllingPawn) <= 1000.f);   // 공격 사거리
    }
    else if (EnemyKnight->Name == "Count")
    {
        bResult = (Target->GetDistanceTo(ControllingPawn) <= 1000.f);   // 공격 사거리
    }
    else if (EnemyKnight->Name == "Guard")
    {
        bResult = (Target->GetDistanceTo(ControllingPawn) <= 1000.f);   // 공격 사거리
    }
    else if (EnemyKnight->Name == "Monarch")
    {
        bResult = (Target->GetDistanceTo(ControllingPawn) <= 1000.f);   // 공격 사거리
    }

    return bResult;
}
