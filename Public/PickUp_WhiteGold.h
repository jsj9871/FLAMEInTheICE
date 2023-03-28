// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_WhiteGold.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API APickUp_WhiteGold : public APickUp
{
	GENERATED_BODY()
	
public:
	APickUp_WhiteGold();

	UFUNCTION()
		virtual void Use_Implementation() override;

	//UPROPERTY(EditInstanceOnly, Category = "Box")
		//TSubclassOf<class APickUp_WhiteGold> ItemClass;
};
