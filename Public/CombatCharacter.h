// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickUp.h"
#include "Interactable.h"
#include "Combat.h"
#include "CombatCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEnemyAttackEndDelegate);

UCLASS(config=Game)
class ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	ACombatCharacter();

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;	// �ִϸ��̼� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void AttackStartComboState();	// �޺� ���� ���� ����
	void AttackEndComboState();	// �޺� ���� ���� ����
	void UseSmallPotion();
	void UseBigPotion();
	void UseDrug();
	void UseDrugEnd();

	//UPROPERTY(VisibleAnywhere, Category = Weapon)
		//UStaticMeshComponent* Weapon;

	int32 GetExp() const;

	void Respawn();

	//bool CanSetTech();	// Tech ���� ���� ���� ����
	void SetTech(class APickUp* SetWeapon);	// Tech ���� ����

	//bool CanSetPurple();	// Bronze ���� ���� ���� ����
	void SetPurple(class APickUp* SetWeapon);	// Bronze ���� ����

	//bool CanSetBronze();	// Bronze ���� ���� ���� ����
	void SetBronze(class APickUp* SetWeapon);	// Bronze ���� ����

	//bool CanSetWhiteGold();	// Bronze ���� ���� ���� ����
	void SetWhiteGold(class APickUp* SetWeapon);	// Bronze ���� ����

	bool CanSetWeapon();

	bool OnTech;
	bool OnPurple;

	UPROPERTY(VisibleAnywhere, Category = Weapon);
	class APickUp* CurWeapon;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DashMontage;	// �뽬 ��Ÿ��

	UPROPERTY(EditAnywhere)
		UAnimMontage* BDMontage;	// �� ������ ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		FString HelpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 Gold;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;	// �⺻ ���� �Ÿ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;	// �⺻ ���� ����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float HurricaneRadius;	// �⺻ ���� ����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float FireSlashRange;	// ȭ���Ӽ� ���� �Ÿ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float IceShoutRange;	// �����Ӽ� ���� �Ÿ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float IceShoutRadius;	// �����Ӽ� ���� �Ÿ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float HurricaneRange;	// �ñر� ���� �Ÿ�

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void UpdateGold(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool AddItemToInventory(APickUp* Item);

	UFUNCTION(BlueprintPure, Category = "Inventory")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintPure, Category = "Inventory")
		FString GetItemNameAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintPure, Category = "Inventory")
		FString GetEquipAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FString UseItemInventorySlot(int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Shop")
		UTexture2D* BuyShopSlot(int32 Slot);

private:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);  // �޺� ���� �̾�� ��������

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;	// ���� ������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsSprinting;	// ������Ʈ ������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = Attack, Meta = (AllowPrivateAccess = true))
		bool OnEquip;	// ���� ���� ������

	UPROPERTY()
		class UPlayerAnimInstance* PAnim;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

private:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;	// ���� �޺� �ܰ� ��������

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;	// ���� �޺� ���� ������

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;	// ���� �޺�

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;	// �ִ� �޺�

public:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UCombatCharacterStatComponent* CurrentStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatPlayerState* PlayerCurrentState;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UEnemyStatComponent* EnemyStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UCombatHUDWidget* HUDWidget;

	//UPROPERTY(VisibleAnywhere, Category = Stat)
		//class ACombatAIController* CombatAIController;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* StaminaBarWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* ExpBarWidget;

	struct FCombatCharacterData* PlayerStatData = nullptr;

public:
	void Sprint();
	void StopSprint();
	void HandleSprint();

	float Speed = 500.f;	// �⺻ �ӵ�
	float SprintSpeed = 1000.f;	// �޸��� �ӵ�

	//void RegenStamina();

	void Dash();	// �� �뽬
	float DashDistance = 2000.f;	// �� �뽬 �Ÿ�

	void BackDive();	// �� ������
	float BDDistance = -1000.f;	// �� ������ �Ÿ�

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void Attack();	// �⺻ ����
	FOnAttackEndDelegate OnAttackEnd;

	void FireSlash();	// ȭ�� �Ӽ� ����
	void FireProjectile(); // ȭ�� ����ü ����
	void IceShout();	// ���� �Ӽ� ����
	void Buff();
	void Hurricane();	// �ñر� ����

	float TotalAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float WeaponAttack;

	float BuffAttack;
	float DrugAttack;

	void GetKnockBack();
	void GetEnableInput();

	void GetSlow();
	void GetMonarchTornadoSlow();

	void GetNotSlow();

	void GetStun();

	// ������ų ������Ÿ�� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACombatProjectile> ProjectileClass;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	float Reach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DashCoolDown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float BackDiveCoolDown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float FireCoolDown = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float FireProjectileCoolDown = 7.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float IceCoolDown = 7.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float BuffCoolDown = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float HurricaneCoolDown = 20.f;

	void ToggleInventory();

	void ToggleEquip();

	void Interact();

	void CheckForInteractable();

	void AttackCheck();	// �⺻ ���� ����
	void FireSlashCheck();	// ȭ�� �Ӽ� ���� ����
	void FireProjectileCheck();
	void IceShoutCheck();	// ���� �Ӽ� ���� ����
	void BuffCheck();	// ���� ����
	void BuffEndCheck();	// ���� �� ����
	void HurricaneCheck();	// �ñر� ���� ����
	void HurricaneEnd();

	void FillDashCoolDown();	// ȭ�� ��ų ��Ÿ�� ä���
	void FillBackDiveCoolDown();	// ���� ��ų ��Ÿ�� ä���

	void FillFireCoolDown();	// ȭ�� ��ų ��Ÿ�� ä���
	void FillFireProjectileCoolDown();	// ȭ��  ����ü ��ų ��Ÿ�� ä���
	void FillIceCoolDown();	// ���� ��ų ��Ÿ�� ä���
	void FillBuffCoolDown();	// ���� ��ų ��Ÿ�� ä���
	void FillHurricaneCoolDown();	// �ñ� ��ų ��Ÿ�� ä���

	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;

	UPROPERTY()
		class ACombatPlayerController* CombatPlayerController;

	UPROPERTY()
		class ACombatAIController* CombatAIController;

	UPROPERTY()
		class ACombatBossController* CombatBossController;

	UPROPERTY()
		class ACombatGuardController* CombatGuardController;

	UPROPERTY()
		class ACombatMonarchController* CombatMonarchController;

	AInteractable* CurrentInteractable;

	UPROPERTY(EditAnywhere)
		TArray<APickUp*> Inventory;

	UPROPERTY(EditAnywhere)
		TArray<APickUp*> Shop;

	FTimerHandle SprintHandle;
	FTimerHandle DashHandle;
	FTimerHandle BackDiveHandle;
	FTimerHandle FillFireHandle;
	FTimerHandle FillFireProjectileHandle;
	FTimerHandle FillIceHandle;
	FTimerHandle FillBuffHandle;
	FTimerHandle FillHurricaneHandle;
	FTimerHandle HurricaneHandle;
	FTimerHandle KnockBackHandle;
	FTimerHandle SlowHandle;
	FTimerHandle MonarchSlowHandle;
	FTimerHandle StunHandle;
	FTimerHandle DrugHandle;
	//FTimerHandle AssetStreamingHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;

	FTimerHandle DeadTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle BuffTimerHandle;

	void OnAssetLoadCompleted();

	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode, Meta = (AllowPrivateAccess = true))
	bool UseGuardKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode, Meta = (AllowPrivateAccess = true))
	bool UseMonarchKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode, Meta = (AllowPrivateAccess = true))
	bool EnemyKnockBack;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }



};