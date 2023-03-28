
#include "EnemyStatComponent.h"
#include "CombatCharacterStatComponent.h"
#include "CombatGameInstance.h"
#include "Combat.h"
#include "EnemyKnight.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Row = 1;
}

void UEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetEnemy(Row);
}

void UEnemyStatComponent::SetEnemy(int32 NewRow)
{
	auto CombatGameInstance = Cast<UCombatGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr != CombatGameInstance)
		EnemyStatData = CombatGameInstance->GetEnemyData(NewRow);
	if (nullptr != EnemyStatData)
	{
		Row = NewRow;
		Name = EnemyStatData->Name;
		Level = EnemyStatData->Level;
		SetHP(EnemyStatData->MaxHP);
		DropExp = EnemyStatData->DropExp;
	}
	else
	{

	}
}

void UEnemyStatComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, EnemyStatData->MaxHP));
}

void UEnemyStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UEnemyStatComponent::RegenHP()
{
	if ((CurrentHP < EnemyStatData->MaxHP))
	{
		CurrentHP += 5.f;
	}
}

int32 UEnemyStatComponent::GetDropExp() const
{
	return EnemyStatData->DropExp;
}

float UEnemyStatComponent::GetAttack()
{
	PCHECK(nullptr != EnemyStatData, 0.0f);

	return EnemyStatData->Attack;
}

float UEnemyStatComponent::GetHPRatio()
{
	PCHECK(nullptr != EnemyStatData, 0.0f);

	return (EnemyStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / EnemyStatData->MaxHP);
}

// Called when the game starts
void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UEnemyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//DropExp();
}

