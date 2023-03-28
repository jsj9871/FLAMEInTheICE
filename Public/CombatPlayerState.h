// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CombatPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    ACombatPlayerState();

    int32 GetCharacterLevel() const;
    int32 GetCharacterIndex() const;

    float GetExpRatio() const;
    bool AddExp(int32 Exp);

    void InitPlayerData();
    void SavePlayerData();
    FString SaveSlotName;

    FOnPlayerStateChangedDelegate OnPlayerStateChanged;

    UPROPERTY(VisibleAnywhere, Category = Effect)
        UParticleSystemComponent* Effect;

public:
    UPROPERTY(Transient)
        int32 CharacterLevel;

    UPROPERTY(Transient)
        int32 CurrentExp;

    UPROPERTY(Transient)
        int32 CharacterIndex;

private:
    void SetCharacterLevel(int32 NewCharacterLevel);

    struct FCombatCharacterData* PlayerStatData;

    //class UCombatCharacterStatComponent* CurrentPlayerStat = nullptr;
};
