// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "CombatAIController.h"
#include "CombatCharacter.h"
#include "EnemyKnight.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
    NodeName = TEXT("Detect");  // 노드 이름
    Interval = 1.0f;    // 간격
}

// 매 프레임 실행
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
        return;

    UWorld* World = ControllingPawn->GetWorld();
    FVector Center = ControllingPawn->GetActorLocation();
    float DetectRadius = 1000.0f;   // 탐지 범위
    auto EnemyKnight = Cast<AEnemyKnight>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == EnemyKnight)
        return;

    if (EnemyKnight->Name == "Guard" || EnemyKnight->Name == "Monarch")
    {
        DetectRadius = 3000.f;
    }

    if (nullptr == World)
        return;

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
    bool bResult = World->OverlapMultiByChannel(
        OverlapResults, 
        Center, 
        FQuat::Identity, 
        ECollisionChannel::ECC_GameTraceChannel6, 
        FCollisionShape::MakeSphere(DetectRadius), 
        CollisionQueryParam); 

    if (bResult)    // 플레이어 탐지 시
    {
        for (auto const& OverlapResult : OverlapResults)
        {
            ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(OverlapResult.GetActor());
            if (CombatCharacter && CombatCharacter->GetController()->IsPlayerController())
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACombatAIController::TargetKey, CombatCharacter);
                //DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);   // 탐지 범위

                //DrawDebugPoint(World, CombatCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);   // 점
                //DrawDebugLine(World, ControllingPawn->GetActorLocation(), CombatCharacter->GetActorLocation(), FColor::Blue, false, 0.27f); // 선
                
                return;
            }
        }

       
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACombatAIController::TargetKey, nullptr);  // 플레이어 위치

    //DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); // 탐지 범위 표시
}

