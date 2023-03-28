// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatBossController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"

const FName ACombatBossController::HomePosKey(TEXT("HomePos"));
const FName ACombatBossController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACombatBossController::TargetKey(TEXT("Target"));

ACombatBossController::ACombatBossController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_Combat.BB_Combat"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_Boss.BT_Boss"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ACombatBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ACombatAIController::OnRepeatTimer, RepeatInterval, true);
	if (UseBlackboard(BBAsset, (UBlackboardComponent*)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			//CLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ACombatBossController::RunAI()
{
	if (UseBlackboard(BBAsset, (UBlackboardComponent*)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
	}
}

void ACombatBossController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
