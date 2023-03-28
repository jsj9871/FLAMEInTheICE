// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCharacterStatComponent.h"
#include "EnemyStatComponent.h"
#include "CombatGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatCharacter.h"
#include "Combat.h"
#include "CombatHUDWidget.h"
#include "CombatPlayerState.h"

// Sets default values for this component's properties
UCombatCharacterStatComponent::UCombatCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	Level = 1;
}

// Called when the game starts
void UCombatCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���ʿ� �� ���� �ݺ�
	GetWorld()->GetTimerManager().SetTimer(HPHandle, this, &UCombatCharacterStatComponent::RegenHP, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UCombatCharacterStatComponent::RegenStamina, 0.5f, true);
}

void UCombatCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UCombatCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto CombatGameInstance = Cast<UCombatGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PCHECK(nullptr != CombatGameInstance);
	PlayerStatData = CombatGameInstance->GetCombatCharacterData(NewLevel);

	if (nullptr != PlayerStatData)
	{
		//auto CombatPlayerState = Cast<ACombatPlayerState>(PlayerState);
		//PCHECK(nullptr != CombatPlayerState);

		Level = NewLevel;
		//CurrentExp = CombatPlayerState->CurrentExp;

		SetHP(PlayerStatData->MaxHP);
		SetStamina(PlayerStatData->MaxStamina);
		auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PCHECK(nullptr != CombatCharacter);
		CurrentAttack = PlayerStatData->Attack + CombatCharacter->WeaponAttack;
	}
}

// HP�� ����� ����
void UCombatCharacterStatComponent::SetDamage(float NewDamage)
{
	PCHECK(nullptr != PlayerStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, PlayerStatData->MaxHP));
}

// HP ����
void UCombatCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)	// ���� ���̱�
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

// HP ����
void UCombatCharacterStatComponent::RegenHP()
{
	PCHECK(nullptr != PlayerStatData);

	if ((CurrentHP < PlayerStatData->MaxHP))
	{
		CurrentHP += 5.f;	// ���� ��
	}
}

// �޸��� ����
void UCombatCharacterStatComponent::SetSprint()
{
	PCHECK(nullptr != PlayerStatData);

	if (CurrentStamina > 0)
	{
		SetStamina(FMath::Clamp<float>(CurrentStamina, 0.0f, PlayerStatData->MaxStamina));
	}
}

// ���׹̳� ����
void UCombatCharacterStatComponent::SetStamina(float NewStamina)
{
	CurrentStamina = NewStamina;
	OnStaminaChanged.Broadcast();
	if (CurrentStamina < KINDA_SMALL_NUMBER)
	{
		CurrentStamina = 0.0f;
		OnStaminaIsZero.Broadcast();
	}
}

// ���׹̳� ����
void UCombatCharacterStatComponent::DecreaseStamina(float Stamina)
{
	if (CurrentStamina > 0.f)
	{
		CurrentStamina -= Stamina;
	}
}

// �޸��� ���� / ���� X
void UCombatCharacterStatComponent::ControlSprint(bool IsSprinting)
{
	if (IsSprinting)	// �޸��� ���� ��
	{
		GetWorld()->GetTimerManager().PauseTimer(StaminaHandle);	// ���׹̳� ����
	}
	else	// �޸��� ���� ��
	{
		GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle);	// ���׹̳� ���� ����
	}
}

// ���׹̳� ����
void UCombatCharacterStatComponent::RegenStamina()
{
	PCHECK(nullptr != PlayerStatData);

	if (CurrentStamina < PlayerStatData->MaxStamina)
	{
		CurrentStamina += 1.f;
	}
}

// ���� ���� ���
void UCombatCharacterStatComponent::UseSmallPotion()
{
	CurrentHP += 300.f;
}

float UCombatCharacterStatComponent::GetAttack()
{
	PCHECK(nullptr != PlayerStatData, 0.0f);

	return PlayerStatData->Attack;
}

// HP ����
float UCombatCharacterStatComponent::GetHPRatio() const
{
	PCHECK(nullptr != PlayerStatData, 0.0f);

	return (PlayerStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / PlayerStatData->MaxHP);
}

float UCombatCharacterStatComponent::GetStaminaRatio() const
{
	PCHECK(nullptr != PlayerStatData, 0.0f);

	return (PlayerStatData->MaxStamina < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentStamina / PlayerStatData->MaxStamina);
}

int32 UCombatCharacterStatComponent::GetDropExp() const
{
	return EnemyStatData->DropExp;
}

// Called every frame
void UCombatCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//PCHECK(HUDWidget != nullptr);
	//HUDWidget->BindCharacterStat(this);
}

