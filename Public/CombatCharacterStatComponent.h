// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStaminaIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStaminaChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UCombatCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	void RegenHP();
	void SetSprint();
	void SetStamina(float NewStamina);
	void DecreaseStamina(float Stamina);
	void ControlSprint(bool IsSprinting);
	void RegenStamina();
	void UseSmallPotion();
	float GetAttack();
	float GetHPRatio() const;
	float GetStaminaRatio() const;
	int32 GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

	FOnStaminaIsZeroDelegate OnStaminaIsZero;
	FOnStaminaChangedDelegate OnStaminaChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class ACombatCharacter* Character = nullptr;

	class UCombatHUDWidget* HUDWidget = nullptr;

	class ACombatPlayerState* PlayerState = nullptr;

public:
	struct FCombatCharacterData* PlayerStatData = nullptr;

	struct FEnemyData* EnemyStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentStamina;

	//UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		//int32 CurrentExp;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentAttack;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float TotalAttack;

/*private:
	UCombatCharacterStatComponent* UCombatCharacterStatComponent::Instance = nullptr;

public:
	UCombatCharacterStatComponent::UCombatCharacterStatComponent(const FObjectInitializer & ObjectInitializer)
		: Super(ObjectInitializer)
	{
		// 최초 호출 된 경우 -> static 변수
		if (nullptr == CharacterCurrentStat)
		{
			CharacterCurrentStat = new UCombatCharacterStatComponent;	// 동적 할당
		}

		return CharacterCurrentStat;
	}
*/

private:
	FTimerHandle HPHandle;
	FTimerHandle StaminaHandle;
};
