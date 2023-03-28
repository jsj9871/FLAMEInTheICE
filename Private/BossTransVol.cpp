// Fill out your copyright notice in the Description page of Project Settings.


#include "BossTransVol.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "CombatCharacter.h"

// Sets default values
ABossTransVol::ABossTransVol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransVol = CreateDefaultSubobject<UBoxComponent>(TEXT("TransVol"));
	RootComponent = TransVol;
	TransVol->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void ABossTransVol::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossTransVol::NotifyActorBeginOver(AActor* OtherActor)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TransLevelName));
	/*
	ACombatCharacter* Character = Cast<ACombatCharacter>(OtherActor);

	if (Character)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TransLevelName));
	}
	*/
}

// Called every frame
void ABossTransVol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

