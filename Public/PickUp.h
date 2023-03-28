// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API APickUp : public AInteractable
{
	GENERATED_BODY()
	
public:
	APickUp();

	void UnEquip();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
		void Use();
	virtual void Use_Implementation();
	virtual void Buy_Implementation();

	UPROPERTY(EditAnywhere, Category = "PickUp")
		UTexture2D* PickUpThumbnail;

	UPROPERTY(EditAnywhere, Category = "Buy")
		UTexture2D* BuyThumbnail;

	UPROPERTY(EditAnywhere, Category = "PickUp")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "PickUp")
		FString Equip;

	UPROPERTY(EditAnywhere, Category = "PickUp")
		int32 Value;

	int32 WeaponCurrentIndex = 0;
	int32 WeaponMaxIndex = 4;

	UPROPERTY(VisibleAnywhere)
		TArray<FString> WeaponsPath;

	UPROPERTY(VisibleAnywhere)
		TArray<USkeletalMesh*> WeaponInven;

	UPROPERTY(EditInstanceOnly, Category = Box)
		TSubclassOf<class APickUp> WeaponItemClass;

	FName Name;
		
	void OnPickedUp();
};
