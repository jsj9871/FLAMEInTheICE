// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMonarchController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"

const FName ACombatMonarchController::HomePosKey(TEXT("HomePos"));
const FName ACombatMonarchController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACombatMonarchController::TargetKey(TEXT("Target"));

ACombatMonarchController::ACombatMonarchController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_Combat.BB_Combat"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_Monarch.BT_Monarch"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ACombatMonarchController::OnPossess(APawn* InPawn)
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

UEnemyBossHUDWidget* ACombatMonarchController::GetHUDWidget() const
{
	return HUDWidget;
}

void ACombatMonarchController::RunAI()
{
	if (UseBlackboard(BBAsset, (UBlackboardComponent*)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
	}
}

void ACombatMonarchController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
