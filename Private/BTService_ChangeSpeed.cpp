// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "EnemyKnight.h"
#include "CombatAIController.h"
#include "Combat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyStatComponent.h"
#include "EnemyBossHUDWidget.h"
#include "GameFramework/Character.h"
#include "CombatGameInstance.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	AEnemyKnight* const Knight = Cast<AEnemyKnight>(cont->GetPawn());
	Knight->GetCharacterMovement()->MaxWalkSpeed = Speed;

	//GetWorld()->GetTimerManager().SetTimer(HPHandle, this, &UBTService_ChangeSpeed::RegenHP, 0.5f, true);
}

/*void UBTService_ChangeSpeed::RegenHP()
{
	PCHECK(nullptr != EnemyStatData);

	if ((EnemyCurrentStat->CurrentHP < EnemyStatData->MaxHP))
	{
		EnemyCurrentStat->CurrentHP += 5.f;
	}
}*/

FString UBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString();
}
