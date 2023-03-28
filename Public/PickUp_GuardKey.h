// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_GuardKey.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API APickUp_GuardKey : public APickUp
{
	GENERATED_BODY()
	
public:
	APickUp_GuardKey();

	UFUNCTION()
		virtual void Use_Implementation() override;
};
