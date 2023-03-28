// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp_Gold.h"
#include "Combat.h"
#include "CombatCharacter.h"

APickUp_Gold::APickUp_Gold()
{
	AmountOfGolds = 0;
}

void APickUp_Gold::Interact_Implementation()
{
	ACombatCharacter * Character = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Character->UpdateGold(AmountOfGolds);
	
	Destroy();
}
