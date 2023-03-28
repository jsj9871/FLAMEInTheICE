// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Combat.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.0f;
	IsInAir = false;
	IsDead = false;

	// �뽬	��Ÿ�� ���� ã�� ����
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DASH_MONTAGE(TEXT("/Game/ParagonYin/Characters/Heroes/Yin/Animations/Dash_Montage.Dash_Montage"));

	if (DASH_MONTAGE.Succeeded())
	{
		DashMontage = DASH_MONTAGE.Object;
	}*/

	// �⺻(�޺�) ���� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/ParagonYin/Characters/Heroes/Yin/Animations/ComboAttack_Montage.ComboAttack_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// ���� �Ӽ� ��ų ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ICESHOUT_MONTAGE(TEXT("/Game/ThirdPerson/Animation/IceShout_Montage.IceShout_Montage"));

	if (ICESHOUT_MONTAGE.Succeeded())
	{
		IceShoutMontage = ICESHOUT_MONTAGE.Object;
	}

	// ȭ�� �Ӽ� ��ų ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FIRESLASH_MONTAGE(TEXT("/Game/ParagonYin/Characters/Heroes/Yin/Animations/FireSlash_Montage.FireSlash_Montage"));

	if (FIRESLASH_MONTAGE.Succeeded())
	{
		FireSlashMontage = FIRESLASH_MONTAGE.Object;
	}

	// ȭ�� �Ӽ� ����ü ��ų ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FIREPROJECTILE_MONTAGE(TEXT("/Game/ThirdPerson/Animation/FireProjectile_Montage.FireProjectile_Montage"));

	if (FIREPROJECTILE_MONTAGE.Succeeded())
	{
		FireProjectileMontage = FIREPROJECTILE_MONTAGE.Object;
	}

	// ���� ��ų ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		BUFF_MONTAGE(TEXT("/Game/ThirdPerson/Animation/Buff_Montage.Buff_Montage"));

	if (BUFF_MONTAGE.Succeeded())
	{
		BuffMontage = BUFF_MONTAGE.Object;
	}

	// �ñر� ��ų ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HURRICANE_MONTAGE(TEXT("/Game/ThirdPerson/Animation/Hurricane_Montage.Hurricane_Montage"));

	if (HURRICANE_MONTAGE.Succeeded())
	{
		HurricaneMontage = HURRICANE_MONTAGE.Object;
	}

	// �ǰ� �������� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HITREACT_MONTAGE(TEXT("/Game/ParagonYin/Characters/Heroes/Yin/Animations/HitReact_Front_Montage.HitReact_Front_Montage"));

	if (HITREACT_MONTAGE.Succeeded())
	{
		HitReactMontage = HITREACT_MONTAGE.Object;
	}

	// �˹� �������� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GETICEKNOCKBACK_MONTAGE(TEXT("/Game/ThirdPerson/Animation/IceKnockBack_Montage.IceKnockBack_Montage"));

	if (GETICEKNOCKBACK_MONTAGE.Succeeded())
	{
		GetIceKnockBackMontage = GETICEKNOCKBACK_MONTAGE.Object;
	}

	// ���� �������� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GETLIGHTSTUN_MONTAGE(TEXT("/Game/ThirdPerson/Animation/LightStun_Montage.LightStun_Montage"));

	if (GETLIGHTSTUN_MONTAGE.Succeeded())
	{
		GetLightStunMontage = GETLIGHTSTUN_MONTAGE.Object;
	}
}

/*void UPlayerAnimInstance::PlayDashMontage()
{
	Montage_Play(DashMontage, 1.0f);
}*/

// �޺� ���� ��Ÿ��
void UPlayerAnimInstance::ComboAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(AttackMontage, 1.0f);
	}
}

// ���� �޺� ���� ��Ÿ�ַ� �Ѿ��
void UPlayerAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	PCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

// ȭ�� ��ų ��Ÿ�� ����
void UPlayerAnimInstance::FireSlash()
{
	if (!Montage_IsPlaying(FireSlashMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(FireSlashMontage, 1.0f);
	}
}

void UPlayerAnimInstance::FireProjectile()
{
	if (!Montage_IsPlaying(FireProjectileMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(FireProjectileMontage, 1.0f);
	}
}

// ���� ��ų ��Ÿ�� ����
void UPlayerAnimInstance::IceShout()
{
	if (!Montage_IsPlaying(IceShoutMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(IceShoutMontage, 1.0f);
	}
}

// ���� ��ų ��Ÿ�� ����
void UPlayerAnimInstance::Buff()
{
	if (!Montage_IsPlaying(BuffMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(BuffMontage, 1.0f);
	}
}

// �ñ� ��ų ��Ÿ�� ����
void UPlayerAnimInstance::Hurricane()
{
	if (!Montage_IsPlaying(HurricaneMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(HurricaneMontage, 1.0f);
	}
}

// Ÿ�� �������� ��Ÿ�� ����
void UPlayerAnimInstance::HitReact()
{
	if (!Montage_IsPlaying(HitReactMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(HitReactMontage, 1.0f);
	}
}

void UPlayerAnimInstance::GetIceKnockBack()
{
	if (!Montage_IsPlaying(GetIceKnockBackMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(GetIceKnockBackMontage, 1.0f);
	}
}

void UPlayerAnimInstance::GetLightStun()
{
	if (!Montage_IsPlaying(GetLightStunMontage))
	{
		PCHECK(!IsDead);
		Montage_Play(GetLightStunMontage, 1.0f);
	}
}

void UPlayerAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UPlayerAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	PCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
