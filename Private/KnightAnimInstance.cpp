#include "KnightAnimInstance.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Combat.h"

UKnightAnimInstance::UKnightAnimInstance()
{
	Speed = 0.0f;
	IsInAir = false;
	IsDead = false;

	// 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Enemy/Squire/Attack_Montage.Attack_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		KnightAttackMontage = ATTACK_MONTAGE.Object;
	}

	// 피격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HITREACT_MONTAGE(TEXT("/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Animations/HitReact_Front_Montage.HitReact_Front_Montage"));

	if (HITREACT_MONTAGE.Succeeded())
	{
		HitReactMontage = HITREACT_MONTAGE.Object;
	}

	// 원거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		VENERETATTACK_MONTAGE(TEXT("/Game/Enemy/Veneret/LongAttack_Montage.LongAttack_Montage"));

	if (VENERETATTACK_MONTAGE.Succeeded())
	{
		VeneretAttackMontage = VENERETATTACK_MONTAGE.Object;
	}

	// 성주 패턴 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		CASTEATTACK_MONTAGE(TEXT("/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Animations/PatternAttackMontage.PatternAttackMontage"));

	if (CASTEATTACK_MONTAGE.Succeeded())
	{
		CasteAttackMontage = CASTEATTACK_MONTAGE.Object;
	}

	// 성주 피격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		CASTEHITREACT_MONTAGE(TEXT("/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Animations/HitReact_Back_Montage.HitReact_Back_Montage"));

	if (CASTEHITREACT_MONTAGE.Succeeded())
	{
		CasteHitReactMontage = CASTEHITREACT_MONTAGE.Object;
	}

	// 남작 근거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		BARONATTACK_MONTAGE(TEXT("/Game/Enemy/Baron/PatternAttack_Montage.PatternAttack_Montage"));

	if (BARONATTACK_MONTAGE.Succeeded())
	{
		BaronAttackMontage = BARONATTACK_MONTAGE.Object;
	}

	// 남작 원거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		BARONLONGATTACK_MONTAGE(TEXT("/Game/Enemy/Baron/LongAttack_Montage.LongAttack_Montage"));

	if (BARONLONGATTACK_MONTAGE.Succeeded())
	{
		BaronLongAttackMontage = BARONLONGATTACK_MONTAGE.Object;
	}

	// 남작 피격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		BARONHITREACT_MONTAGE(TEXT("/Game/ParagonGideon/Characters/Heroes/Gideon/Animations/HitReact_Back_Montage.HitReact_Back_Montage"));

	if (BARONHITREACT_MONTAGE.Succeeded())
	{
		BaronHitReactMontage = BARONHITREACT_MONTAGE.Object;
	}

	// 백작 근거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		COUNTATTACK_MONTAGE(TEXT("/Game/Enemy/Count/PatternAttack.PatternAttack"));

	if (COUNTATTACK_MONTAGE.Succeeded())
	{
		CountAttackMontage = COUNTATTACK_MONTAGE.Object;
	}

	// 백작 원거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		COUNTLONGATTACK_MONTAGE(TEXT("/Game/Enemy/Count/IceStunAttack_Montage.IceStunAttack_Montage"));

	if (COUNTLONGATTACK_MONTAGE.Succeeded())
	{
		CountLongAttackMontage = COUNTLONGATTACK_MONTAGE.Object;
	}

	// 백작 궁극기 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		COUNTULTI_MONTAGE(TEXT("/Game/Enemy/Count/CountUltimate_Montage.CountUltimate_Montage"));

	if (COUNTULTI_MONTAGE.Succeeded())
	{
		CountUltiMontage = COUNTULTI_MONTAGE.Object;
	}

	// 군주 호위무사 패턴 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GUARDATTACK_MONTAGE(TEXT("/Game/Enemy/Guard/PatternAttack.PatternAttack"));

	if (GUARDATTACK_MONTAGE.Succeeded())
	{
		GuardAttackMontage = GUARDATTACK_MONTAGE.Object;
	}

	// 군주 호위무사 원거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GUARDLONGATTACK_MONTAGE(TEXT("/Game/Enemy/Guard/LongAttack_Montage.LongAttack_Montage"));

	if (GUARDLONGATTACK_MONTAGE.Succeeded())
	{
		GuardLongAttackMontage = GUARDLONGATTACK_MONTAGE.Object;
	}

	// 군주 호위무사 궁극기 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GUARDULTI_MONTAGE(TEXT("/Game/Enemy/Guard/GuardUlti_Montage.GuardUlti_Montage"));

	if (GUARDULTI_MONTAGE.Succeeded())
	{
		GuardUltiMontage = GUARDULTI_MONTAGE.Object;
	}

	// 군주 근거리 공격 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONARCHATTACK_MONTAGE(TEXT("/Game/Enemy/Monarch/MonarchAttack_Montage.MonarchAttack_Montage"));

	if (MONARCHATTACK_MONTAGE.Succeeded())
	{
		MonarchAttackMontage = MONARCHATTACK_MONTAGE.Object;
	}

	// 군주 라인 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONARCHLINE_MONTAGE(TEXT("/Game/Enemy/Monarch/MonarchStrike_Montage.MonarchStrike_Montage"));

	if (MONARCHLINE_MONTAGE.Succeeded())
	{
		MonarchStrikeMontage = MONARCHLINE_MONTAGE.Object;
	}

	// 군주 버스트 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONARCHBURST_MONTAGE(TEXT("/Game/Enemy/Monarch/Burst_Montage.Burst_Montage"));

	if (MONARCHBURST_MONTAGE.Succeeded())
	{
		MonarchBurstMontage = MONARCHBURST_MONTAGE.Object;
	}

	// 군주 토네이도 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONARCHTORNADO_MONTAGE(TEXT("/Game/Enemy/Monarch/MonarchTornado_Montage.MonarchTornado_Montage"));

	if (MONARCHTORNADO_MONTAGE.Succeeded())
	{
		MonarchTornadoMontage = MONARCHTORNADO_MONTAGE.Object;
	}

	// 군주 궁극기 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONARCHULTI_MONTAGE(TEXT("/Game/Enemy/Monarch/Ultimate_Montage.Ultimate_Montage"));

	if (MONARCHULTI_MONTAGE.Succeeded())
	{
		MonarchUltiMontage = MONARCHULTI_MONTAGE.Object;
	}

	// 군주 궁극기 몽타주 삽입
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GETKNOCKBACK_MONTAGE(TEXT("/Game/Enemy/Squire/KnockBack_Montage.KnockBack_Montage"));

	if (GETKNOCKBACK_MONTAGE.Succeeded())
	{
		GetKnockBackMontage = GETKNOCKBACK_MONTAGE.Object;
	}
}

void UKnightAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		Speed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UKnightAnimInstance::AttackMontage()
{
	PCHECK(!IsDead);
	Montage_Play(KnightAttackMontage, 1.0f);
}

void UKnightAnimInstance::HitReact()
{
	PCHECK(!IsDead);
	Montage_Play(HitReactMontage, 1.0f);
}

void UKnightAnimInstance::GetKnockBack()
{
	PCHECK(!IsDead);
	Montage_Play(GetKnockBackMontage, 1.0f);
}

void UKnightAnimInstance::VeneretAttack()
{
	PCHECK(!IsDead);
	Montage_Play(VeneretAttackMontage, 1.0f);
}

void UKnightAnimInstance::CasteAttack()
{
	PCHECK(!IsDead);
	Montage_Play(CasteAttackMontage, 1.0f);
}

void UKnightAnimInstance::CasteHitReact()
{
	PCHECK(!IsDead);
	Montage_Play(CasteHitReactMontage, 1.0f);
}

void UKnightAnimInstance::BaronAttack()
{
	PCHECK(!IsDead);
	Montage_Play(BaronAttackMontage, 1.0f);
}

void UKnightAnimInstance::BaronLongAttack()
{
	PCHECK(!IsDead);
	Montage_Play(BaronLongAttackMontage, 1.0f);
}

void UKnightAnimInstance::BaronHitReact()
{
	PCHECK(!IsDead);
	Montage_Play(BaronHitReactMontage, 1.0f);
}

void UKnightAnimInstance::CountAttack()
{
	PCHECK(!IsDead);
	Montage_Play(CountAttackMontage, 1.0f);
}

void UKnightAnimInstance::CountLongAttack()
{
	PCHECK(!IsDead);
	Montage_Play(CountLongAttackMontage, 1.0f);
}

void UKnightAnimInstance::CountUlti()
{
	PCHECK(!IsDead);
	Montage_Play(CountUltiMontage, 1.0f);
}

void UKnightAnimInstance::GuardAttack()
{
	PCHECK(!IsDead);
	Montage_Play(GuardAttackMontage, 1.0f);
}

void UKnightAnimInstance::GuardLongAttack()
{
	PCHECK(!IsDead);
	Montage_Play(GuardLongAttackMontage, 1.0f);
}

void UKnightAnimInstance::GuardUlti()
{
	PCHECK(!IsDead);
	Montage_Play(GuardUltiMontage, 1.0f);
}

void UKnightAnimInstance::MonarchAttack()
{
	PCHECK(!IsDead);
	Montage_Play(MonarchAttackMontage, 1.0f);
}

void UKnightAnimInstance::MonarchStrikeAttack()
{
	PCHECK(!IsDead);
	Montage_Play(MonarchStrikeMontage, 1.0f);
}

void UKnightAnimInstance::MonarchBurstAttack()
{
	PCHECK(!IsDead);
	Montage_Play(MonarchBurstMontage, 1.0f);
}

void UKnightAnimInstance::MonarchTornadoAttack()
{
	PCHECK(!IsDead);
	Montage_Play(MonarchTornadoMontage, 1.0f);
}

void UKnightAnimInstance::MonarchUltiAttack()
{
	PCHECK(!IsDead);
	Montage_Play(MonarchUltiMontage, 1.0f);
}

void UKnightAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_EliteAttackCheck()
{
	OnEliteAttackCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_EliteMaxAttackCheck()
{
	OnEliteMaxAttackCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_BossAttackCheck()
{
	OnBossAttackCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_BossMaxAttackCheck()
{
	OnBossMaxAttackCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_BaronTornadoCheck()
{
	OnBaronTornadoCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_BaronLaserCheck()
{
	OnBaronLaserCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_CountBlastCheck()
{
	OnCountBlastCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_GuardBallCheck()
{
	OnGuardBallCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_GuardMaxAttackCheck()
{
	OnGuardMaxAttackCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_MonarchStrikeCheck()
{
	OnMonarchStrikeCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_MonarchBluntCheck()
{
	OnMonarchBluntCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_MonarchTornadoCheck()
{
	OnMonarchTornadoCheck.Broadcast();
}

void UKnightAnimInstance::AnimNotify_MonarchUltiCheck()
{
	OnMonarchUltiCheck.Broadcast();
}
