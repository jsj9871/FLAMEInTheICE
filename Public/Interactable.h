// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class COMBAT_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION(BlueprintNativeEvent)
		void Interact();

	virtual void Interact_Implementation();

	UPROPERTY(EditAnywhere, Category = "Interactable")
		class USkeletalMeshComponent* InteractableMesh;

	UPROPERTY(EditAnywhere, Category = "Interactable")
		class UStaticMeshComponent* PotionMesh;

	UPROPERTY(EditAnywhere, Category = "Interactable")
		FString InteractableHelpText;	
};
