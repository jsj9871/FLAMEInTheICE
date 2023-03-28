// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "CombatAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
    NodeName = TEXT("FindPatrolPos");   // ��� �̸�
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (nullptr == NavSystem)
        return EBTNodeResult::Failed;

    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ACombatAIController::HomePosKey);
    FNavLocation NextPatrol;

    if (NavSystem->GetRandomPointInNavigableRadius(Origin, 300.f, NextPatrol)) // �ִ� ���� �Ÿ�
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(ACombatAIController::PatrolPosKey, NextPatrol.Location);   // ���� ��ġ
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
