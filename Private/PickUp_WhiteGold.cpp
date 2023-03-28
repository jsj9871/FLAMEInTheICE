// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp_WhiteGold.h"
#include "CombatCharacter.h"
#include "Combat.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

APickUp_WhiteGold::APickUp_WhiteGold()
{
    // Tech 무기 불러오기
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/ThirdPerson/Blueprints/PickUps/Weapon/WhiteGold.WhiteGold"));
    if (SK_WEAPON.Succeeded())
    {
        InteractableMesh->SetSkeletalMesh(SK_WEAPON.Object);
    }

    //InteractableMesh->SetCollisionProfileName(TEXT("NoCollision"));

    RootComponent = InteractableMesh;

    WeaponItemClass = APickUp_WhiteGold::StaticClass();
}

void APickUp_WhiteGold::Use_Implementation()
{
    auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    PCHECK(nullptr != CombatCharacter);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Equip"));
    auto SetWeapon = GetWorld()->SpawnActor<APickUp>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
    CombatCharacter->SetWhiteGold(SetWeapon);
}
