// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("Turn");    // 노드 이름
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto CombatCharacter = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());   // 몬스터가
    if (nullptr == CombatCharacter)
        return EBTNodeResult::Failed;

    auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatAIController::TargetKey));
    if (nullptr == Target)  // 플레이어를
        return EBTNodeResult::Failed;

    // 바라보게 함
    FVector LookVector = Target->GetActorLocation() - CombatCharacter->GetActorLocation();
    LookVector.Z = 0.0f;
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    CombatCharacter->SetActorRotation(FMath::RInterpTo(CombatCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

    return EBTNodeResult::Succeeded;
}
