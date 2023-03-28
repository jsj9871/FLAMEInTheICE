// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "PickUp_Bronze.generated.h"

/**
 *
 */
UCLASS()
class COMBAT_API APickUp_Bronze : public APickUp
{
	GENERATED_BODY()

public:
	APickUp_Bronze();

	UFUNCTION()
	virtual void Use_Implementation() override;
};
