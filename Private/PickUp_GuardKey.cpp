// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_GuardKey.h"
#include "CombatCharacter.h"
#include "Combat.h"

APickUp_GuardKey::APickUp_GuardKey()
{

}

void APickUp_GuardKey::Use_Implementation()
{
	auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PCHECK(nullptr != CombatCharacter);

	CombatCharacter->UseGuardKey = true;
}
