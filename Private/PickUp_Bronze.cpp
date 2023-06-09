
#include "PickUp_Bronze.h"
#include "CombatCharacter.h"
#include "Combat.h"
#include "CombatCharacterStatComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

APickUp_Bronze::APickUp_Bronze()
{
    // Bronze 무기 불러오기
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/ThirdPerson/Blueprints/PickUps/Weapon/Bronze.Bronze"));
    if (SK_WEAPON.Succeeded())
    {
        InteractableMesh->SetSkeletalMesh(SK_WEAPON.Object);
    }

    //InteractableMesh->SetCollisionProfileName(TEXT("NoCollision"));

    RootComponent = InteractableMesh;

    WeaponItemClass = APickUp_Bronze::StaticClass();
}

void APickUp_Bronze::Use_Implementation()
{
    auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    PCHECK(nullptr != CombatCharacter);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Equip"));
    auto SetWeapon = GetWorld()->SpawnActor<APickUp>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
    CombatCharacter->SetBronze(SetWeapon);
}