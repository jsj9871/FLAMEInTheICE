// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ItemBox.generated.h"

UCLASS()
class COMBAT_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;

};
