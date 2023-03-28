// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Regen.h"
#include "EnemyStatComponent.h"
#include "Combat.h"
#include "CombatGameInstance.h"

UBTService_Regen::UBTService_Regen()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Regen");
}

void UBTService_Regen::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();

	GetWorld()->GetTimerManager().SetTimer(RegenHPHandle, this, &UBTService_Regen::RegenHP, 0.5f, true);
}

void UBTService_Regen::RegenHP()
{
	//PCHECK(nullptr != EnemyStatData);
	//PCHECK(nullptr != EnemyCurrentStat);
	/*
	if ((EnemyCurrentStat->CurrentHP < EnemyStatData->MaxHP))
	{
		EnemyCurrentStat->CurrentHP += 5.f;
	}
	*/
}
