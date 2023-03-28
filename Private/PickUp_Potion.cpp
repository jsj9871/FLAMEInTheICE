// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Potion.h"
#include "CombatCharacter.h"
#include "Combat.h"

APickUp_Potion::APickUp_Potion()
{
    /*
    // SmallPotion �ҷ�����
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ST_POTION(TEXT("/Game/ThirdPerson/Blueprints/PickUps/Potion/SmallPotion.SmallPotion"));
    if (ST_POTION.Succeeded())
    {
        //PotionMesh->SetStaticMesh(ST_POTION.Object);
    }

    RootComponent = PotionMesh;
    */
    
}

void APickUp_Potion::Use_Implementation()
{
    auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    PCHECK(nullptr != CombatCharacter);

    CombatCharacter->UseSmallPotion();
}
