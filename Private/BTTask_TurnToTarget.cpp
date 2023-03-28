// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("Turn");    // ��� �̸�
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto CombatCharacter = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());   // ���Ͱ�
    if (nullptr == CombatCharacter)
        return EBTNodeResult::Failed;

    auto Target = Cast<ACombatCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACombatAIController::TargetKey));
    if (nullptr == Target)  // �÷��̾
        return EBTNodeResult::Failed;

    // �ٶ󺸰� ��
    FVector LookVector = Target->GetActorLocation() - CombatCharacter->GetActorLocation();
    LookVector.Z = 0.0f;
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    CombatCharacter->SetActorRotation(FMath::RInterpTo(CombatCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

    return EBTNodeResult::Succeeded;
}
