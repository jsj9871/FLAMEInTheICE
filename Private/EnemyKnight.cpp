// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyKnight.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KnightAnimInstance.h"
#include "EnemyStatComponent.h"
#include "CombatGameInstance.h"
#include "CombatProjectile.h"
#include "EnemyWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "CombatCharacterStatComponent.h"
#include "CombatPlayerController.h"
#include "CombatAIController.h"
#include "Kismet/GameplayStatics.h"
#include "CombatCharacter.h"
#include "CombatDamageText.h"
#include "Combat.h"
#include "CombatMonarchController.h"
#include "CombatAIController.h"
#include "CombatBossController.h"
#include "CombatGuardController.h"
#include "EnemyBossHUDWidget.h"
#include "PickUp.h"
#include "EngineUtils.h"

// Sets default values
AEnemyKnight::AEnemyKnight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	IsAttacking = false;

	AttackRange = 100.f;
	AttackRadius = 50.f;

	EliteAttackRange = 200.f;

	EliteMaxAttackRange = 250.f;
	EliteMaxAttackRadius = 100.f;

	BossAttackRange = 250.f;
	BossAttackRadius = 100.f;

	BossMaxAttackRange = 300.f;
	BossMaxAttackRadius = 150.f;

	GuardLongAttackRange = 1000.f;
	GuardLongAttackRadius = 250.f;
	GuardUltiRadius = 700.f;

	CountBlastRange = 500.f;
	CountBlastRadius = 100.f;

	CountUltiRadius = 850.f;

	MonarchBurstRadius = 270.f;
	MonarchStrikeRange = 400.f;
	MonarchStrikeRadius = 50.f;
	MonarchTornadoRadius = 1000.f;
	MonarchUltiRadius = 600.f;

	DeadTimer = 1.5f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyCurrentStat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("CHARACTERSTAT"));

	EnemyHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ENEMYHPWIDGET"));
	EnemyHPWidget->SetupAttachment(GetMesh());
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	EnemyHPWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/ThirdPerson/Blueprints/HUDs/BP_EnemyHUDBar.BP_EnemyHUDBar_C"));
	if (UI_HUD.Succeeded())
	{
		EnemyHPWidget->SetWidgetClass(UI_HUD.Class);
		EnemyHPWidget->SetDrawSize(FVector2D(150.0f, 30.0f));
		EnemyHPWidget->SetHiddenInGame(true);
	}

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	if (Name == "Monarch")
	{
		FName WeaponSocket(TEXT("WeaponParticle"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

			static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/Enemy/Monarch/WeaponParticle.WeaponParticle"));
			if (P_CHESTOPEN.Succeeded())
			{
				Effect->SetTemplate(P_CHESTOPEN.Object);
				Effect->bAutoActivate = true;
			}

			Effect->SetupAttachment(GetMesh(), WeaponSocket);
		}
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> TECH(TEXT("Blueprint'/Game/ThirdPerson/Blueprints/PickUps/Weapon/BP_Tech.BP_Tech'"));
	if (TECH.Object)
	{
		TechClass = (UClass*)TECH.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> PURPLE(TEXT("Blueprint'/Game/ThirdPerson/Blueprints/PickUps/Weapon/BP_Purple.BP_Purple'"));
	if (PURPLE.Object)
	{
		PurpleClass = (UClass*)PURPLE.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> BRONZE(TEXT("Blueprint'/Game/ThirdPerson/Blueprints/PickUps/Weapon/BP_Bronze.BP_Bronze'"));
	if (BRONZE.Object)
	{
		BronzeClass = (UClass*)BRONZE.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> WHITEGOLD(TEXT("Blueprint'/Game/ThirdPerson/Blueprints/PickUps/Weapon/BP_WhiteGold.BP_WhiteGold'"));
	if (WHITEGOLD.Object)
	{
		WhiteGoldClass = (UClass*)WHITEGOLD.Object->GeneratedClass;
	}
}

void AEnemyKnight::SetCharacterState(ECharacterState NewState)
{
	PCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::PREINIT:
	{

	}
	break;

	case ECharacterState::LOADING:
	{
		//CombatMonarchController->GetHUDWidget()->BindEnemyStat(EnemyCurrentStat);
		//SetActorHiddenInGame(true);
		//HPBarWidget->SetHiddenInGame(true);
		//SetCanBeDamaged(false);

		//auto CombatPlayerState = Cast<ACombatPlayerState>(GetPlayerState());
		//PCHECK(nullptr != CombatPlayerState);
		//CurrentStat->SetNewLevel(CombatPlayerState->GetCharacterLevel());

		//auto CharacterWidget = Cast<UCombatCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		//PCHECK(nullptr != CharacterWidget);
		//CharacterWidget->BindCharacterStat(CurrentStat);
	}
	break;

	case ECharacterState::READY:
	{
		//SetActorHiddenInGame(false);
		//HPBarWidget->SetHiddenInGame(false);
		//SetCanBeDamaged(true);
		//CurrentStat->OnHPIsZero.AddLambda([this]() ->void {
			//SetCharacterState(ECharacterState::DEAD);
			//});
		//auto CharacterWidget = Cast<UCombatCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		//PCHECK(nullptr != CharacterWidget);
		//CharacterWidget->BindCharacterStat(CurrentStat);
	}
	break;

	case ECharacterState::DEAD:
	{
		//SetActorEnableCollision(false);
		//GetMesh()->SetHiddenInGame(false);
		//HPBarWidget->SetHiddenInGame(true);
		//PAnim->SetDeadAnim();
		//SetCanBeDamaged(false);
	}
	break;
	}
}

ECharacterState AEnemyKnight::GetCharacterState() const
{
	return CurrentState;
}

// Called when the game starts or when spawned
void AEnemyKnight::BeginPlay()
{
	Super::BeginPlay();
	
	auto EnemyWidget = Cast<UEnemyWidget>(EnemyHPWidget->GetUserWidgetObject());
	if (nullptr != EnemyWidget)
	{
		EnemyWidget->BindEnemyStat(EnemyCurrentStat);
	}
	
	CombatMonarchController = Cast<ACombatMonarchController>(GetController());
	if (nullptr != CombatMonarchController)
	{
		CombatMonarchController->GetHUDWidget()->BindEnemyStat(EnemyCurrentStat);
	}

	AIController = Cast<ACombatAIController>(GetController());

	BossController = Cast<ACombatBossController>(GetController());

	GuardController = Cast<ACombatGuardController>(GetController());

	//SetCharacterState(ECharacterState::LOADING);
}

// Called every frame
void AEnemyKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyKnight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 몬스터 정보
void AEnemyKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KAnim = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	PCHECK(nullptr != KAnim);

	KAnim->OnMontageEnded.AddDynamic(this, &AEnemyKnight::OnAttackMontageEnded);

	KAnim->OnAttackHitCheck.AddUObject(this, &AEnemyKnight::AttackCheck);

	KAnim->OnEliteAttackCheck.AddUObject(this, &AEnemyKnight::EliteAttackCheck);
	KAnim->OnEliteMaxAttackCheck.AddUObject(this, &AEnemyKnight::EliteMaxAttackCheck);

	KAnim->OnBossAttackCheck.AddUObject(this, &AEnemyKnight::BossAttackCheck);
	KAnim->OnBossMaxAttackCheck.AddUObject(this, &AEnemyKnight::BossMaxAttackCheck);

	KAnim->OnBaronTornadoCheck.AddUObject(this, &AEnemyKnight::BaronLongAttackCheck);
	KAnim->OnBaronLaserCheck.AddUObject(this, &AEnemyKnight::BaronLaserCheck);

	KAnim->OnCountBlastCheck.AddUObject(this, &AEnemyKnight::CountBlastCheck);

	KAnim->OnGuardBallCheck.AddUObject(this, &AEnemyKnight::GuardBallCheck);
	KAnim->OnGuardMaxAttackCheck.AddUObject(this, &AEnemyKnight::GuardMaxAttackCheck);

	KAnim->OnMonarchStrikeCheck.AddUObject(this, &AEnemyKnight::MonarchStrikeCheck);
	KAnim->OnMonarchBluntCheck.AddUObject(this, &AEnemyKnight::MonarchBluntCheck);
	KAnim->OnMonarchTornadoCheck.AddUObject(this, &AEnemyKnight::MonarchTornadoCheck);
	KAnim->OnMonarchUltiCheck.AddUObject(this, &AEnemyKnight::MonarchUltiCheck);
	
	if (Name == "Squire")
	{
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		SpawnBaguette();
		GetWorldTimerManager().SetTimer(SpawnEnemyHandle, this, &AEnemyKnight::SpawnEnemy, 5.f, false);
		FindSquireSpawnPoints();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle1, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Peong")
	{
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		SpawnBaguette();
		GetWorldTimerManager().SetTimer(SpawnEnemyHandle, this, &AEnemyKnight::SpawnEnemy, 5.f, false);
		FindPeongSpawnPoints();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle2, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Veneret")
	{
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		SpawnBaguette();
		GetWorldTimerManager().SetTimer(SpawnEnemyHandle, this, &AEnemyKnight::SpawnEnemy, 5.f, false);
		FindVeneretSpawnPoints();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle3, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Castellan")
	{
		//AIController = Cast<ACombatAIController>(GetController());
		//PCHECK(nullptr != AIController);
		//AIController->StopAI();
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		if (nullptr != AIController)
		{
			AIController->StopAI();
		}
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		SpawnPurple();
		SpawnChicken();
		GetWorldTimerManager().SetTimer(SpawnEliteHandle, this, &AEnemyKnight::SpawnEnemy, 5.f, false);
		FindCastellanSpawnPoints();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle4, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Baron")
	{
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		if (nullptr != BossController)
		{
			BossController->StopAI();
		}
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		SpawnBronze();
		SpawnChicken();
		GetWorldTimerManager().SetTimer(SpawnEliteHandle, this, &AEnemyKnight::SpawnEnemy, 5.f, false);
		FindBaronSpawnPoints();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle5, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Count")
	{
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		if (nullptr != BossController)
		{
			BossController->StopAI();
		}
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		SpawnWhiteGold();
		SpawnChicken();
		SpawnCandy();
		FindCountSpawnPoints();
		GetWorldTimerManager().SetTimer(SpawnEliteHandle, this, &AEnemyKnight::SpawnEnemy, 5.f, false);
		FindGuardKeySpawnPoints();
		SpawnGuardKey();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle6, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Guard")
	{
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		if (nullptr != GuardController)
		{
			GuardController->StopAI();
		}
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		SetCharacterState(ECharacterState::DEAD);
		GetWorldTimerManager().SetTimer(SpawnBossHandle, this, &AEnemyKnight::SpawnEnemy, 9.f, false);
		SpawnMonarchKey();
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle7, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
			});
	}
	else if (Name == "Monarch")
	{
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("StoryEnd"));
		// 몬스터 HP가 0이 되었을 때
		EnemyCurrentStat->OnHPIsZero.AddLambda([this]()->void {KAnim->SetDeadAnim();
		if (nullptr != CombatMonarchController)
		{
			CombatMonarchController->StopAI();
		}
		SetActorEnableCollision(false);	// 콜리전 삭제
		GetMesh()->SetHiddenInGame(false);	// 메쉬 삭제
		EnemyHPWidget->SetHiddenInGame(true);	// HP 위젯 삭제
		GetWorldTimerManager().SetTimer(SpawnBossHandle, this, &AEnemyKnight::SpawnEnemy, 9.f, false);
		SetCharacterState(ECharacterState::DEAD);
		Controller->Destroy();	// 컨트롤러 삭제
		// 5초 뒤 삭제
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle8, FTimerDelegate::CreateLambda([this]()->void {Destroy(); }), DeadTimer, false);
		SetActorLocation(FVector(-4630.0f, 4680.0f, -87.f));
			});
	}
}

float AEnemyKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Name == "Squire" || Name == "Peong" || Name == "Veneret")
	{
		KAnim->HitReact();
	}
	else if (Name == "Castellan")
	{
		//KAnim->CasteHitReact();
	}
	else if (Name == "Baron")
	{
		//KAnim->BaronHitReact();
	}

	EnemyHPWidget->SetHiddenInGame(false);
	GetWorldTimerManager().SetTimer(HPWidgetHandle, this, &AEnemyKnight::HPVisibility, 10.f, false);

	FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	EnemyCurrentStat->SetDamage(FinalDamage);

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto PlayerController = Cast<ACombatPlayerController>(EventInstigator);
			PCHECK(nullptr != PlayerController, 0.0f);
			PlayerController->EnemyKill(this);
		}
	}

	return FinalDamage;
}

int32 AEnemyKnight::GetExp() const
{
	return EnemyCurrentStat->GetDropExp();
}

/*int32 AEnemyKnight::GetDropExp() const
{
	return EnemyCurrentStat->DropExp;
}*/

// 근거리 공격
void AEnemyKnight::Attack()
{
	if (!GetCharacterMovement()->IsFalling())	// 점프 상태 제외
	{
		GetCharacterMovement()->DisableMovement();	// 움직임 정지

		if (Name == "Squire" || Name == "Peong")
		{
			KAnim->AttackMontage();	// 공격 몽타주
		}
		else if (Name == "Castellan")
		{
			KAnim->CasteAttack();
		}
		else if (Name == "Baron")
		{
			KAnim->BaronAttack();
		}
		else if (Name == "Count")
		{
			KAnim->CountAttack();
		}
		else if (Name == "Guard")
		{
			KAnim->GuardAttack();
		}
		else if (Name == "Monarch")
		{
			KAnim->MonarchAttack();
		}

		IsAttacking = true;

		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
	}
}

void AEnemyKnight::GetKnockBack()
{
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));

	KAnim->GetKnockBack();

	GetWorldTimerManager().SetTimer(GetKnockBackHandle, this, &AEnemyKnight::GetEnableInput, 2.f, false);
}

void AEnemyKnight::GetEnableInput()
{
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}

// 원거리 공격
void AEnemyKnight::LongAttack()
{
	if (!GetCharacterMovement()->IsFalling())	// 점프 상태 제외
	{
		GetCharacterMovement()->DisableMovement();	// 움직임 정지
		IsAttacking = true;

		if (Name == "Veneret")
		{
			KAnim->VeneretAttack();

			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(50.f, 20.f, 10.0f));	// 공격체 스폰 위치
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->SpawnActor<ACombatProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
		}
		else if (Name == "Baron")
		{
			KAnim->BaronLongAttack();

			//GetWorld()->GetTimerManager().SetTimer(BaronLongAttackHandle, this, &AEnemyKnight::BaronLongAttackCheck, 1.f, false);
		}
		else if (Name == "Count")
		{
			KAnim->CountLongAttack();

			GetWorld()->GetTimerManager().SetTimer(CountLongAttackHandle, this, &AEnemyKnight::CountLongAttackCheck, 0.5f, false);
		}
		else if (Name == "Guard")
		{
			KAnim->GuardLongAttack();

			// 2번 공격
			GetWorld()->GetTimerManager().SetTimer(GuardLongAttackHandle, this, &AEnemyKnight::GuardLongAttackCheck, 0.3f, true, 1.f);
			GetWorldTimerManager().SetTimer(GuardLongHandle, this, &AEnemyKnight::GuardLongEnd, 1.4f, false);
		}

		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
	}
}

void AEnemyKnight::BaronLongAttackCheck()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(20.f, 20.f, 10.f));	// 공격체 스폰 위치
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<ACombatProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	}
}

void AEnemyKnight::BaronLaserCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1000.0f,
		ECollisionChannel::ECC_GameTraceChannel6,
		Params,
		FCollisionResponseParams());

#if ENABLE_DRAW_DEBUG
	FVector Start = GetActorLocation();
	FVector End = GetActorForwardVector() * 1000.f + Start;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugLine(GetWorld(), Start, End, DrawColor, false, DebugLifeTime, 0.f, 5.f);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::CountLongAttackCheck()
{
	const FRotator SpawnRotation1 = GetActorRotation();
	const FVector SpawnLocation1 = GetActorLocation() + SpawnRotation1.RotateVector(FVector(-50.f, 200.f, 10.f));	// 공격체 스폰 위치
	const FRotator SpawnRotation2 = GetActorRotation();
	const FVector SpawnLocation2 = GetActorLocation() + SpawnRotation2.RotateVector(FVector(20.f, 0.f, 10.f));	// 공격체 스폰 위치
	const FRotator SpawnRotation3 = GetActorRotation();
	const FVector SpawnLocation3 = GetActorLocation() + SpawnRotation3.RotateVector(FVector(-50.f, -200.f, 10.f));	// 공격체 스폰 위치
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<ACombatProjectile>(ProjectileClass, SpawnLocation1, SpawnRotation1);
		World->SpawnActor<ACombatProjectile>(ProjectileClass2, SpawnLocation2, SpawnRotation2);
		World->SpawnActor<ACombatProjectile>(ProjectileClass3, SpawnLocation3, SpawnRotation3);
	}
}

// 기본 공격 정보
void AEnemyKnight::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 100.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.0f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5 + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::HPVisibility()
{
	EnemyHPWidget->SetHiddenInGame(true);
}

// 기본 공격 정보
void AEnemyKnight::EliteAttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.0f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * EliteAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = EliteAttackRange * 0.5 + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::EliteMaxAttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 250.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(100.f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * EliteMaxAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = EliteMaxAttackRange * 0.5 + EliteMaxAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, EliteMaxAttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack() * 1.5, DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::BossAttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 250.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(100.f),
		Params);
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * BossAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = BossAttackRange * 0.5 + BossAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::BossMaxAttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 300.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(150.f),
		Params);
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * BossMaxAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = BossMaxAttackRange * 0.5 + BossMaxAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, BossMaxAttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	
#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack() * 1.5, DamageEvent, GetController(), this);	// 대미지
	}
}

// 백작 궁극 스킬
void AEnemyKnight::CountUlti()
{
	if (!GetCharacterMovement()->IsFalling())	// 점프 상태 제외
	{
		GetCharacterMovement()->DisableMovement();	// 움직임 정지

		if (!IsAttacking)
		{
			GetWorld()->GetTimerManager().SetTimer(CountUltiTimerHandle, FTimerDelegate::CreateLambda([this]()->void {CountUltiCheck(); }), 0.8f, false);	// 공격 실행
			KAnim->CountUlti();	// 공격 몽타주
		}

		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
	}
}

void AEnemyKnight::GuardLongEnd()
{
	GetWorldTimerManager().ClearTimer(GuardLongAttackHandle);
}

void AEnemyKnight::GuardUlti()
{
	if (!GetCharacterMovement()->IsFalling())	// 점프 상태 제외
	{
		GetCharacterMovement()->DisableMovement();	// 움직임 정지

		if (!IsAttacking)
		{
			KAnim->GuardUlti();	// 공격 몽타주

			auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			PCHECK(nullptr != CombatCharacter);

			CombatCharacter->GetSlow();

			// 4번 공격
			GetWorld()->GetTimerManager().SetTimer(GuardUltiTimerHandle, FTimerDelegate::CreateLambda([this]()->void {GuardUltiCheck(); }), 0.5f, true, 3.5f);	// 공격 실행
			GetWorld()->GetTimerManager().SetTimer(GuardUltiHandle, this, &AEnemyKnight::GuardUltiEnd, 5.5f, false);
		}

		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
	}
}

void AEnemyKnight::GuardUltiEnd()
{
	GetWorldTimerManager().ClearTimer(GuardUltiTimerHandle);
}

void AEnemyKnight::FillCountUltiCoolDown()
{
	CountUltiCoolDown += 0.1f;
}

void AEnemyKnight::CountUltiCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() - GetActorUpVector() * 800.f,
		GetActorLocation() - GetActorUpVector() * 800.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(850.f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector Center = GetActorLocation() - GetActorUpVector() * 800.f;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugSphere(GetWorld(), Center, CountUltiRadius, 16, DrawColor, false, DebugLifeTime, 5.f);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지

		auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PCHECK(nullptr != CombatCharacter);

		CombatCharacter->GetKnockBack();
	}
}

void AEnemyKnight::CountBlastCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 500.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(100.f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * CountBlastRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = CountBlastRange * 0.5 + CountBlastRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, CountBlastRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::GuardLongAttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() - GetActorUpVector() * 250.f,
		GetActorLocation() - GetActorUpVector() * 250.f + GetActorForwardVector() * 1000.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(250.f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * GuardLongAttackRange;
	FVector Center = GetActorLocation() - GetActorUpVector() * 250.f + TraceVec * 0.5f;
	float HalfHeight = GuardLongAttackRange * 0.5 + GuardLongAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, GuardLongAttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::GuardMaxAttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + GetActorForwardVector() * 300.f,
		GetActorLocation() + GetActorForwardVector() * 600.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(150.f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * BossMaxAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = BossMaxAttackRange * 0.5 + BossMaxAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, BossMaxAttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack() * 1.5, DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::GuardBallCheck()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(20.f, 20.f, 150.f));	// 공격체 스폰 위치
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<ACombatProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	}
}

void AEnemyKnight::GuardUltiCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(700.f),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector Center = GetActorLocation();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.f;

	DrawDebugSphere(GetWorld(), Center, GuardUltiRadius, 16, DrawColor, false, DebugLifeTime, 5.f);

#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::MonarchBurst()
{
	if (!GetCharacterMovement()->IsFalling())	// 점프 상태 제외
	{
		GetCharacterMovement()->DisableMovement();	// 움직임 정지

		if (!IsAttacking)
		{
			GetWorld()->GetTimerManager().SetTimer(MonarchBurstHandle, this, &AEnemyKnight::MonarchBurstCheck, 1.f, false);	// 공격 실행
			KAnim->MonarchBurstAttack();	// 공격 몽타주
		}

		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
	}
}

void AEnemyKnight::MonarchStrike()
{
	GetCharacterMovement()->DisableMovement();	// 움직임 정지

	if (!IsAttacking)
	{
		KAnim->MonarchStrikeAttack();	// 공격 몽타주
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
}

void AEnemyKnight::MonarchTornado()
{
	GetCharacterMovement()->DisableMovement();	// 움직임 정지

	if (!IsAttacking)
	{
		KAnim->MonarchTornadoAttack();	// 공격 몽타주
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
}

void AEnemyKnight::MonarchUlti()
{
	GetCharacterMovement()->DisableMovement();	// 움직임 정지

	if (!IsAttacking)
	{
		KAnim->MonarchUltiAttack();	// 공격 몽타주
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
}

void AEnemyKnight::MonarchBurstCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() - GetActorUpVector() * 200.f + GetActorForwardVector() * 500.f,
		GetActorLocation() - GetActorUpVector() * 200.f + GetActorForwardVector() * 500.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(270.f),
		Params);
	/*
#if ENABLE_DRAW_DEBUG
	FVector Center = GetActorLocation() - GetActorUpVector() * 200.f + GetActorForwardVector() * 500.f;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.f;

	DrawDebugSphere(GetWorld(), Center, MonarchBurstRadius, 16, DrawColor, false, DebugLifeTime, 5.f);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack() * 1.7, DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::MonarchStrikeCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + GetActorForwardVector() * 700.f + GetActorUpVector() * 400.f,
		GetActorLocation() + GetActorForwardVector() * 700.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f),
		Params);
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = - GetActorUpVector() * MonarchStrikeRange;
	FVector Center = GetActorLocation() + GetActorForwardVector() * 700.f + GetActorUpVector() * 400.f + TraceVec * 0.5f;
	float HalfHeight = MonarchStrikeRange * 0.5 + MonarchStrikeRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, MonarchStrikeRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지

		auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PCHECK(nullptr != CombatCharacter);

		CombatCharacter->GetStun();
	}
}

void AEnemyKnight::MonarchBluntCheck()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.f, 20.f, -100.f));	// 공격체 스폰 위치
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<ACombatProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	}
}

void AEnemyKnight::MonarchTornadoCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() - GetActorUpVector() * 200.f,
		GetActorLocation() - GetActorUpVector() * 200.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(1000.f),
		Params);
	/*
#if ENABLE_DRAW_DEBUG
	FVector Center = GetActorLocation() - GetActorUpVector() * 200.f;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.f;

	DrawDebugSphere(GetWorld(), Center, MonarchTornadoRadius, 16, DrawColor, false, DebugLifeTime, 5.f);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack() * 0.1, DamageEvent, GetController(), this);	// 대미지

		auto CombatCharacter = Cast<ACombatCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PCHECK(nullptr != CombatCharacter);

		CombatCharacter->GetMonarchTornadoSlow();
	}
}

void AEnemyKnight::MonarchUltiCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() - GetActorUpVector() * 200.f + GetActorForwardVector() * 400.f,
		GetActorLocation() - GetActorUpVector() * 200.f + GetActorForwardVector() * 400.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(600.f),
		Params);
	/*
#if ENABLE_DRAW_DEBUG
	FVector Center = GetActorLocation() - GetActorUpVector() * 200.f + GetActorForwardVector() * 400.f;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.f;

	DrawDebugSphere(GetWorld(), Center, MonarchUltiRadius, 16, DrawColor, false, DebugLifeTime, 5.f);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;

		HitResult.GetActor()->TakeDamage(EnemyCurrentStat->GetAttack(), DamageEvent, GetController(), this);	// 대미지
	}
}

void AEnemyKnight::SpawnEnemy()
{
	int index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	//UGameplayStatics::GetAllActorsOfClass(world, GeneratedBP, FoundActors);
	GetWorld()->SpawnActor<AEnemyKnight>(SpawnEnemyClass, SpawnPoints[index]->GetActorLocation(), FRotator(0));
}

void AEnemyKnight::SpawnGold()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.f;
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<AActor>(SpawnGoldClass, SpawnLocation, SpawnRotation);
	}
}

void AEnemyKnight::FindSquireSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_SquireSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindPeongSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_PeongSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindVeneretSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_VeneretSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindCastellanSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_CastellanSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindBaronSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_BaronSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindCountSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_CountSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindGuardSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_GuardSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindMonarchSpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_MonarchSpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::FindGuardKeySpawnPoints()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors)
	{
		if (spawn->GetName().Contains(TEXT("BP_GuardKeySpawnPoint")))
		{
			SpawnPoints.Add(spawn);
		}
	}
}

void AEnemyKnight::SpawnTech()
{
	int32 Random = FMath::RandRange(1, 2);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 30.f - GetActorUpVector() * 100.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(TechClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnPurple()
{
	int32 Random = FMath::RandRange(1, 3);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 30.f - GetActorUpVector() * 100.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(PurpleClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnBronze()
{
	int32 Random = FMath::RandRange(1, 4);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 30.f - GetActorUpVector() * 50.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(BronzeClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnWhiteGold()
{
	int32 Random = FMath::RandRange(1, 5);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 30.f - GetActorUpVector() * 50.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(WhiteGoldClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnBaguette()
{
	int32 Random = FMath::RandRange(1, 2);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.f - GetActorUpVector() * 50.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(BaguetteClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnChicken()
{
	int32 Random = FMath::RandRange(1, 3);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.f - GetActorUpVector() * 50.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(ChickenClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnCandy()
{
	int32 Random = FMath::RandRange(1, 3);

	if (Random == 1)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 10.f - GetActorUpVector() * 50.f;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<APickUp>(CandyClass, SpawnLocation, SpawnRotation);
		}
	}
}

void AEnemyKnight::SpawnGuardKey()
{
	int index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	//UGameplayStatics::GetAllActorsOfClass(world, GeneratedBP, FoundActors);
	GetWorld()->SpawnActor<APickUp>(GuardKeyClass, SpawnPoints[index]->GetActorLocation(), FRotator(0));
}

void AEnemyKnight::SpawnMonarchKey()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 10.f - GetActorUpVector() * 50.f;
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<APickUp>(MonarchKeyClass, SpawnLocation, SpawnRotation);
	}
}

void AEnemyKnight::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}
