// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStatComponent();
	virtual void InitializeComponent() override;

public:
	void SetEnemy(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	void RegenHP();
	int32 GetDropExp() const;
	float GetAttack();
	float GetHPRatio();

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

public:
	struct FEnemyData* EnemyStatData = nullptr;

	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Row;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		FString Name;

	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;

	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 DropExp;



public:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class AEnemyKnight* EnemyKnight;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UCombatCharacterStatComponent* PlayerCurrentStat;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
