// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_ChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	//void RegenHP();

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UEnemyStatComponent* EnemyCurrentStat;

	struct FEnemyData* EnemyStatData = nullptr;

protected:
	FString GetStaticServiceDescription() const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float Speed = 600.f;

	FTimerHandle HPHandle;
};
