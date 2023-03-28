// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerGameMode.h"
#include "CombatPawn.h"
#include "CombatPlayerController.h"

ACombatPlayerGameMode::ACombatPlayerGameMode()
{
    DefaultPawnClass = ACombatPawn::StaticClass();

    PlayerControllerClass = ACombatPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
    if (BP_PAWN_C.Succeeded())
    {
        DefaultPawnClass = BP_PAWN_C.Class;
    }
}