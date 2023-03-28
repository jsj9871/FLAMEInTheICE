// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_MonarchKey.h"
#include "CombatCharacter.h"
#include "Combat.h"

APickUp_MonarchKey::APickUp_MonarchKey()
{

}

void APickUp_MonarchKey::Use_Implementation()
{
	auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PCHECK(nullptr != CombatCharacter);

	CombatCharacter->UseMonarchKey = true;
}
