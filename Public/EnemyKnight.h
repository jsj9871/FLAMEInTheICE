// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat.h"
#include "EnemyKnight.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class COMBAT_API AEnemyKnight : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyKnight();

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	int32 GetExp() const;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	void GetKnockBack();
	void GetEnableInput();
	float FinalDamage;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float EliteAttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float EliteMaxAttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float EliteMaxAttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float BossAttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float BossAttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float BossMaxAttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float BossMaxAttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float CountBlastRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float CountBlastRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float CountUltiRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float GuardLongAttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float GuardLongAttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float GuardUltiRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float MonarchBurstRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float MonarchStrikeRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float MonarchStrikeRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float MonarchTornadoRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float MonarchUltiRadius;

	UFUNCTION(BlueprintCallable)
		void LongAttack();

	UFUNCTION(BlueprintCallable)
		void CountUlti();

	UFUNCTION(BlueprintCallable)
		void GuardLongEnd();

	UFUNCTION(BlueprintCallable)
		void GuardUlti();

	UFUNCTION(BlueprintCallable)
		void GuardUltiEnd();

	UFUNCTION(BlueprintCallable)
		void MonarchBurst();

	UFUNCTION(BlueprintCallable)
		void MonarchStrike();

	UFUNCTION(BlueprintCallable)
		void MonarchTornado();

	UFUNCTION(BlueprintCallable)
		void MonarchUlti();

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACombatProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACombatProjectile> ProjectileClass2;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ACombatProjectile> ProjectileClass3;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
		TSubclassOf<class AActor> DamageTextClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class AEnemyKnight> SpawnEnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class AActor> SpawnGoldClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> TechClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> PurpleClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> BronzeClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> WhiteGoldClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> BaguetteClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> ChickenClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> CandyClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> GuardKeyClass;

	UPROPERTY(EditDefaultsOnly, Category = PickUp)
		TSubclassOf<class APickUp> MonarchKeyClass;

	UPROPERTY(EditDefaultsOnly, Category = MyState)
		FName Name;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
		class UBehaviorTree* Behavior;

	struct FEnemyData* EnemyStatData = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UEnemyStatComponent * EnemyCurrentStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatAIController* AIController;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatBossController* BossController;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatGuardController* GuardController;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatMonarchController* CombatMonarchController;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UWidgetComponent* EnemyHPWidget;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UUserWidget* MonarchHPWidget;

public:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UCombatCharacterStatComponent* PlayerCurrentStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatCharacter* Player;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class ACombatDamageText* CombatDamageText;

private:
	//static UCombatCharacterStatComponent* CharacterCurrentStat;

	//static UEnemyStatComponent* EnemyStat;

/*public:
    static UCombatCharacterStatComponent* GetInstance()
    {
    	// 최초 호출 된 경우 -> static 변수
		if (nullptr == CharacterCurrentStat)
		{
			CharacterCurrentStat = new UCombatCharacterStatComponent;	// 동적 할당
		}
        
        return CharacterCurrentStat;
    }*/

private:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		//UAnimMontage* montage;

	UPROPERTY()
		class UKnightAnimInstance* KAnim;

	UPROPERTY()
		class AEnemyKnight* EK;

	UPROPERTY()
		UParticleSystemComponent* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;

	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor* > SpawnPoints;

	FTimerHandle DeadTimerHandle1 = { };
	FTimerHandle DeadTimerHandle2 = { };
	FTimerHandle DeadTimerHandle3 = { };
	FTimerHandle DeadTimerHandle4 = { };
	FTimerHandle DeadTimerHandle5 = { };
	FTimerHandle DeadTimerHandle6 = { };
	FTimerHandle DeadTimerHandle7 = { };
	FTimerHandle DeadTimerHandle8 = { };
	FTimerHandle BaronLongAttackHandle;
	FTimerHandle BaronLaserHandle;
	FTimerHandle CountLongAttackHandle;
	FTimerHandle GuardLongAttackHandle;
	FTimerHandle GuardLongHandle;
	FTimerHandle GuardUltiHandle;
	FTimerHandle MonarchBurstHandle;
	FTimerHandle GetKnockBackHandle;

	FTimerHandle SpawnEnemyHandle;
	FTimerHandle SpawnEliteHandle;
	FTimerHandle SpawnBossHandle;

	FTimerHandle CountUltiTimerHandle;
	FTimerHandle GuardUltiTimerHandle;
	FTimerHandle FillCountUltiHandle;

	FTimerHandle HPWidgetHandle;

	void HPVisibility();

	float CountUltiCoolDown = 2.f;

	void FillCountUltiCoolDown();

	void AttackCheck();
	void EliteAttackCheck();
	void EliteMaxAttackCheck();
	void BossAttackCheck();
	void BossMaxAttackCheck();

	void BaronLongAttackCheck();
	void BaronLaserCheck();

	void CountLongAttackCheck();
	void CountUltiCheck();
	void CountBlastCheck();

	void GuardLongAttackCheck();
	void GuardMaxAttackCheck();
	void GuardBallCheck();
	void GuardUltiCheck();

	void MonarchBurstCheck();
	void MonarchStrikeCheck();
	void MonarchBluntCheck();
	void MonarchTornadoCheck();
	void MonarchUltiCheck();

	void SpawnEnemy();
	void FindSquireSpawnPoints();
	void FindPeongSpawnPoints();
	void FindVeneretSpawnPoints();
	void FindCastellanSpawnPoints();
	void FindBaronSpawnPoints();
	void FindCountSpawnPoints();
	void FindGuardSpawnPoints();
	void FindMonarchSpawnPoints();
	void FindGuardKeySpawnPoints();
	void SpawnGold();
	void SpawnTech();
	void SpawnPurple();
	void SpawnBronze();
	void SpawnWhiteGold();
	void SpawnBaguette();
	void SpawnChicken();
	void SpawnCandy();
	void SpawnGuardKey();
	void SpawnMonarchKey();

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;
};
