// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_Tech.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API APickUp_Tech : public APickUp
{
	GENERATED_BODY()

public:
	APickUp_Tech();

	UFUNCTION()
	virtual void Use_Implementation() override;

	//UPROPERTY(EditInstanceOnly, Category = "Box")
	//TSubclassOf<class APickUp_Tech> ItemClass;

	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//UStaticMeshComponent* Weapon;

	UPROPERTY()
		class UCombatHUDWidget* CombatHUDWidget;
};
