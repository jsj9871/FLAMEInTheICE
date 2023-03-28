// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatProjectile.generated.h"

UCLASS()
class COMBAT_API ACombatProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = MyState)
		FName Name;

	// 구체 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		class UBoxComponent* CollisionComponent;

	// 프로젝타일 무브먼트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult);

	// 프로젝타일의 속도를 발사 방향으로 초기화
	void FireInDirection(const FVector& ShootDirection);

	// 프로젝타일에 무언가 맞으면 호출
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, AController* GetController(), FVector NormalImpulse, const FHitResult& Hit);
};
