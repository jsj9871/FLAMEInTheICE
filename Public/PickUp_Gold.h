// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_Gold.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API APickUp_Gold : public APickUp
{
	GENERATED_BODY()

	APickUp_Gold();

	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere, Category = "Gold")
	int32 AmountOfGolds;
};
