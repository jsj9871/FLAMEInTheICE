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
	virtual void PostInitializeComponents() override;	// 애니메이션 관리
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void AttackStartComboState();	// 콤보 공격 상태 시작
	void AttackEndComboState();	// 콤보 공격 상태 종료
	void UseSmallPotion();
	void UseBigPotion();
	void UseDrug();
	void UseDrugEnd();

	//UPROPERTY(VisibleAnywhere, Category = Weapon)
		//UStaticMeshComponent* Weapon;

	int32 GetExp() const;

	void Respawn();

	//bool CanSetTech();	// Tech 무기 장착 가능 여부
	void SetTech(class APickUp* SetWeapon);	// Tech 무기 장착

	//bool CanSetPurple();	// Bronze 무기 장착 가능 여부
	void SetPurple(class APickUp* SetWeapon);	// Bronze 무기 장착

	//bool CanSetBronze();	// Bronze 무기 장착 가능 여부
	void SetBronze(class APickUp* SetWeapon);	// Bronze 무기 장착

	//bool CanSetWhiteGold();	// Bronze 무기 장착 가능 여부
	void SetWhiteGold(class APickUp* SetWeapon);	// Bronze 무기 장착

	bool CanSetWeapon();

	bool OnTech;
	bool OnPurple;

	UPROPERTY(VisibleAnywhere, Category = Weapon);
	class APickUp* CurWeapon;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DashMontage;	// 대쉬 몽타주

	UPROPERTY(EditAnywhere)
		UAnimMontage* BDMontage;	// 뒷 구르기 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		FString HelpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 Gold;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;	// 기본 공격 거리

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;	// 기본 공격 넓이

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float HurricaneRadius;	// 기본 공격 넓이

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float FireSlashRange;	// 화염속성 공격 거리

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float IceShoutRange;	// 얼음속성 공격 거리

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float IceShoutRadius;	// 얼음속성 공격 거리

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float HurricaneRange;	// 궁극기 공격 거리

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
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);  // 콤보 공격 이어가지 못했을때

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;	// 공격 중인지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsSprinting;	// 스프린트 중인지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = Attack, Meta = (AllowPrivateAccess = true))
		bool OnEquip;	// 무기 장착 중인지

	UPROPERTY()
		class UPlayerAnimInstance* PAnim;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

private:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;	// 다음 콤보 단계 가능한지

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;	// 현재 콤보 공격 중인지

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;	// 현재 콤보

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;	// 최대 콤보

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

	float Speed = 500.f;	// 기본 속도
	float SprintSpeed = 1000.f;	// 달리기 속도

	//void RegenStamina();

	void Dash();	// 앞 대쉬
	float DashDistance = 2000.f;	// 앞 대쉬 거리

	void BackDive();	// 뒷 구르기
	float BDDistance = -1000.f;	// 뒷 구르기 거리

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void Attack();	// 기본 공격
	FOnAttackEndDelegate OnAttackEnd;

	void FireSlash();	// 화염 속성 공격
	void FireProjectile(); // 화염 투사체 공격
	void IceShout();	// 얼음 속성 공격
	void Buff();
	void Hurricane();	// 궁극기 공격

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

	// 스폰시킬 프로젝타일 클래스
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

	void AttackCheck();	// 기본 공격 정보
	void FireSlashCheck();	// 화염 속성 공격 정보
	void FireProjectileCheck();
	void IceShoutCheck();	// 얼음 속성 공격 정보
	void BuffCheck();	// 버프 정보
	void BuffEndCheck();	// 버프 끝 정보
	void HurricaneCheck();	// 궁극기 공격 정보
	void HurricaneEnd();

	void FillDashCoolDown();	// 화염 스킬 쿨타임 채우기
	void FillBackDiveCoolDown();	// 얼음 스킬 쿨타임 채우기

	void FillFireCoolDown();	// 화염 스킬 쿨타임 채우기
	void FillFireProjectileCoolDown();	// 화염  투사체 스킬 쿨타임 채우기
	void FillIceCoolDown();	// 얼음 스킬 쿨타임 채우기
	void FillBuffCoolDown();	// 버프 스킬 쿨타임 채우기
	void FillHurricaneCoolDown();	// 궁극 스킬 쿨타임 채우기

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