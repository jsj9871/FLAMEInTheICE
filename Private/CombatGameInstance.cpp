// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatGameInstance.h"
#include "Combat.h"

UCombatGameInstance::UCombatGameInstance()
{
    // 스텟 데이터 적용
    FString CharacterDataPath = TEXT("/Game/ThirdPerson/Data/CombatCharacterData.CombatCharacterData");
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_COMBATCHARACTER(*CharacterDataPath);
    PCHECK(DT_COMBATCHARACTER.Succeeded());
    CombatCharacterTable = DT_COMBATCHARACTER.Object;
    PCHECK(CombatCharacterTable->GetRowMap().Num() > 0);

    FString EnemyDataPath = TEXT("/Game/ThirdPerson/Data/EnemyData.EnemyData");
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY(*EnemyDataPath);
    PCHECK(DT_ENEMY.Succeeded());
    EnemyTable = DT_ENEMY.Object;
    PCHECK(EnemyTable->GetRowMap().Num() > 0);
}

void UCombatGameInstance::Init()
{
	Super::Init();

    CLOG(Warning, TEXT("% d"), GetEnemyData(8)->DropExp);
}

FCombatCharacterData* UCombatGameInstance::GetCombatCharacterData(int32 Level)
{
    return CombatCharacterTable->FindRow<FCombatCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FEnemyData* UCombatGameInstance::GetEnemyData(int32 Row)
{
    return EnemyTable->FindRow<FEnemyData>(*FString::FromInt(Row), TEXT(""));
}
