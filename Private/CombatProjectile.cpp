// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatProjectile.h"
#include "CombatCharacter.h"
#include "CombatProjectile.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Pawn.h"

// Sets default values
ACombatProjectile::ACombatProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 상자를 단순 콜리전 표현으로 사용
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACombatProjectile::OnOverlapBegin);

    // 상자의 콜리전 반경을 설정
    if (Name == "BaronAttack")
    {
        CollisionComponent->InitBoxExtent(FVector(6.f, 6.f, 6.f));
    }
    else if (Name == "CountAttack1")
    {
        CollisionComponent->InitBoxExtent(FVector(6.f, 6.f, 12.f));
    }


    // 루트 컴포넌트를 콜리전 컴포넌트로 설정
    RootComponent = CollisionComponent;

    // 프로젝타일의 무브먼트를 구동
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 1000.0f;
    ProjectileMovementComponent->MaxSpeed = 1000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.0f;
}

// Called when the game starts or when spawned
void ACombatProjectile::BeginPlay()
{
	Super::BeginPlay();
	
    // 1.5초 후 삭제
    SetLifeSpan(1.5f);
}

// Called every frame
void ACombatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->IsA(ACombatCharacter::StaticClass()))
        {
            Destroy();
        }
    }
}

// 공격체 중력, 스폰될 때 스피드
void ACombatProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// 공격체가 플레이어에게 적중했을 때
void ACombatProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, AController* GetController(), FVector NormalImpulse, const FHitResult& Hit)
{
    FHitResult HitResult;
    FDamageEvent DamageEvent;

    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        if (Name == "VeneretAttack")
        {
            HitResult.GetActor()->TakeDamage(30.0f, DamageEvent, GetController(), this);    // 대미지
        }
        else if (Name == "BaronAttack")
        {
            HitResult.GetActor()->TakeDamage(100.0f, DamageEvent, GetController(), this);    // 대미지
        }
        else if (Name == "CountAttack")
        {
            HitResult.GetActor()->TakeDamage(150.0f, DamageEvent, GetController(), this);    // 대미지
        }
        HitResult.GetActor()->TakeDamage(100.0f, DamageEvent, GetController(), this);    // 대미지

        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 20.0f, Hit.ImpactPoint);
        Destroy();
    }
}

