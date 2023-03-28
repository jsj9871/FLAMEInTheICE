#include "Interactable.h"
// Fill out your copyright notice in the Description page of Project Settings.

// Sets default values
AInteractable::AInteractable()
{
	//PrimaryActorTick.bCanEverTick = true;

	InteractableHelpText = FString("Press F to interact with item"); 
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

/*void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

void AInteractable::Interact_Implementation()
{
	GLog->Log("Interact base class: Interact() YOU SHOULD NOT BE SEEING THIS");
}

