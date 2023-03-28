// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerState.h"
#include "CombatGameInstance.h"
#include "Combat.h"
#include "CombatCharacterStatComponent.h"
#include "CombatSaveGame.h"

ACombatPlayerState::ACombatPlayerState()
{
	CharacterLevel = 1;
	CurrentExp = 0;
	CharacterIndex = 0;

	SaveSlotName = TEXT("Player1");

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	Effect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> LEVELUP(TEXT("/Game/Effect/LevelUp.LevelUp"));
	if (LEVELUP.Succeeded())
	{
		Effect->SetTemplate(LEVELUP.Object);
		Effect->bAutoActivate = false;
	}
}

int32 ACombatPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 ACombatPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

float ACombatPlayerState::GetExpRatio() const
{
	if (PlayerStatData->MaxExp <= KINDA_SMALL_NUMBER)
		return 0;

	float Result = (float)CurrentExp / (float)PlayerStatData->MaxExp;

	return Result;
}

bool ACombatPlayerState::AddExp(int32 Exp)
{
	if (PlayerStatData->MaxExp == -1)
		return false;

	bool LevelUp = false;
	CurrentExp += Exp;
	if (CurrentExp >= PlayerStatData->MaxExp)
	{
		CurrentExp -= PlayerStatData->MaxExp;
		SetCharacterLevel(CharacterLevel + 1);
		Effect->bAutoActivate = true;
		LevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();

	SavePlayerData();

	return LevelUp;
}

void ACombatPlayerState::InitPlayerData()
{
	auto CombatSaveGame = Cast<UCombatSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == CombatSaveGame)
	{
		CombatSaveGame = GetMutableDefault<UCombatSaveGame>();
	}

	SetPlayerName(CombatSaveGame->PlayerName);
	SetCharacterLevel(CombatSaveGame->Level);
	CurrentExp = CombatSaveGame->CurrentExp;
	CharacterIndex = CombatSaveGame->CharacterIndex;

	SavePlayerData();
}

void ACombatPlayerState::SavePlayerData()
{
	UCombatSaveGame* NewPlayerData = NewObject<UCombatSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->CurrentExp = CurrentExp;
	NewPlayerData->CharacterIndex = CharacterIndex;
}

void ACombatPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto CombatGameInstance = Cast<UCombatGameInstance>(GetGameInstance());
	PCHECK(nullptr != CombatGameInstance);
	
	PlayerStatData = CombatGameInstance->GetCombatCharacterData(NewCharacterLevel);
	PCHECK(nullptr != PlayerStatData);

	CharacterLevel = NewCharacterLevel;
}
