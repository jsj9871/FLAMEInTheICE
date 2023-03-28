// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Drug.h"
#include "CombatCharacter.h"
#include "Combat.h"

APickUp_Drug::APickUp_Drug()
{
    /*
    // SmallPotion 불러오기
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ST_POTION(TEXT("/Game/ThirdPerson/Blueprints/PickUps/Potion/SmallPotion.SmallPotion"));
    if (ST_POTION.Succeeded())
    {
        //PotionMesh->SetStaticMesh(ST_POTION.Object);
    }

    RootComponent = PotionMesh;
    */

}

void APickUp_Drug::Use_Implementation()
{
    auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    PCHECK(nullptr != CombatCharacter);

    CombatCharacter->UseDrug();
}
