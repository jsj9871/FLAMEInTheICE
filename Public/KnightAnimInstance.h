// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KnightAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEliteAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEliteMaxAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBossAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBossMaxAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBaronTornadoCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBaronLaserCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCountBlastCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGuardBallCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGuardMaxAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMonarchStrikeCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMonarchBluntCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMonarchTornadoCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMonarchUltiCheckDelegate);

UCLASS()
class COMBAT_API UKnightAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UKnightAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void AttackMontage();	// 傍拜 根鸥林
	void HitReact();	// 傍拜 根鸥林
	void GetKnockBack();

	void VeneretAttack();

	void CasteAttack();
	void CasteHitReact();

	void BaronAttack();
	void BaronLongAttack();
	void BaronHitReact();

	void CountAttack();
	void CountLongAttack();
	void CountUlti();

	void GuardAttack();
	void GuardLongAttack();
	void GuardUlti();

	void MonarchAttack();
	void MonarchStrikeAttack();
	void MonarchBurstAttack();
	void MonarchTornadoAttack();
	void MonarchUltiAttack();

	void SetDeadAnim()
	{
		IsDead = true;
	}

public:
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnAttackHitCheckDelegate OnEliteAttackCheck;
	FOnAttackHitCheckDelegate OnEliteMaxAttackCheck;
	FOnAttackHitCheckDelegate OnBossAttackCheck;
	FOnAttackHitCheckDelegate OnBossMaxAttackCheck;
	FOnAttackHitCheckDelegate OnBaronTornadoCheck;
	FOnAttackHitCheckDelegate OnBaronLaserCheck;
	FOnAttackHitCheckDelegate OnCountBlastCheck;
	FOnAttackHitCheckDelegate OnGuardBallCheck;
	FOnAttackHitCheckDelegate OnGuardMaxAttackCheck;
	FOnAttackHitCheckDelegate OnMonarchStrikeCheck;
	FOnAttackHitCheckDelegate OnMonarchBluntCheck;
	FOnAttackHitCheckDelegate OnMonarchTornadoCheck;
	FOnAttackHitCheckDelegate OnMonarchUltiCheck;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GetKnockBackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* KnightAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitReactMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* VeneretAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CasteAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CasteHitReactMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BaronAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BaronLongAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BaronHitReactMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CountAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CountLongAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CountUltiMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GuardAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GuardLongAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GuardUltiMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MonarchAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MonarchStrikeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MonarchBurstMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MonarchTornadoMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MonarchUltiMontage;

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_EliteAttackCheck();

	UFUNCTION()
		void AnimNotify_EliteMaxAttackCheck();

	UFUNCTION()
		void AnimNotify_BossAttackCheck();

	UFUNCTION()
		void AnimNotify_BossMaxAttackCheck();

	UFUNCTION()
		void AnimNotify_BaronTornadoCheck();

	UFUNCTION()
		void AnimNotify_BaronLaserCheck();

	UFUNCTION()
		void AnimNotify_CountBlastCheck();

	UFUNCTION()
		void AnimNotify_GuardBallCheck();

	UFUNCTION()
		void AnimNotify_GuardMaxAttackCheck();

	UFUNCTION()
		void AnimNotify_MonarchStrikeCheck();

	UFUNCTION()
		void AnimNotify_MonarchBluntCheck();

	UFUNCTION()
		void AnimNotify_MonarchTornadoCheck();

	UFUNCTION()
		void AnimNotify_MonarchUltiCheck();
};
