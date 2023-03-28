// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"
#include "Combat.h"
#include "Interactable.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CombatCharacter.h"


APickUp::APickUp()
{
	InteractableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickUpMesh"));

	ItemName = FString("Enter an item name here...");
	InteractableHelpText = FString("%s");
	Value = 0;
}

void APickUp::UnEquip()
{
	//InteractableMesh->DetachFromParent();
}

void APickUp::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s"), * ItemName);
}

void APickUp::Interact_Implementation()
{
	ACombatCharacter* Character = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Character->AddItemToInventory(this))
	{
		OnPickedUp();
	}
}

void APickUp::Use_Implementation()
{
	GLog->Log("Use() from base pickup class (YOU SHOULD NOT BE SEEING THIS)");
}

void APickUp::Buy_Implementation()
{

}

void APickUp::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


