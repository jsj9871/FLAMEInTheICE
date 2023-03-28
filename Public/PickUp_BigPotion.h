// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_BigPotion.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API APickUp_BigPotion : public APickUp
{
	GENERATED_BODY()
	
public:
	APickUp_BigPotion();

	UFUNCTION()
		virtual void Use_Implementation() override;
};
