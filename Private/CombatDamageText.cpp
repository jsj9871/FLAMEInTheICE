// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatDamageText.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "EnemyKnight.h"
#include "Kismet/GameplayStatics.h"
#include "Combat.h"

// Sets default values
ACombatDamageText::ACombatDamageText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));

	//auto EnemyKnight = Cast<AEnemyKnight>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//PCHECK(nullptr != EnemyKnight);
}
// Called when the game starts or when spawned
void ACombatDamageText::BeginPlay()
{
	Super::BeginPlay();
	
	//GetWorldTimerManager().SetTimer(DamageTextHandle, this, &ACombatDamageText::TextDestroy, 0.5f, false);
}

// Called every frame
void ACombatDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatDamageText::TextDestroy()
{
	Destroy();
}

