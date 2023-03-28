// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "CombatCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Combat.h"
//#include "blackboard_keys.h"

const FName ACombatAIController::HomePosKey(TEXT("HomePos"));
const FName ACombatAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACombatAIController::TargetKey(TEXT("Target"));

ACombatAIController::ACombatAIController()
{
	//RepeatInterval = 3.0f;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_Combat.BB_Combat"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_Combat.BT_Combat"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ACombatAIController::OnPossess(APawn* InPawn)
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

void ACombatAIController::RunAI()
{
	if (UseBlackboard(BBAsset, (UBlackboardComponent*)Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
	}
}

void ACombatAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

/*void ACombatAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}*/

/*void ACombatAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	PCHECK(nullptr != CurrentPawn);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) {
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 1000.0f, NextLocation))
	{
		//UNavigationSystemV1::SimpleMoveToLocation(this, NextLocation.Location);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
	}
}*/