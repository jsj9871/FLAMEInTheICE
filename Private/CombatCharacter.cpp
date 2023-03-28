// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAnimInstance.h"
#include "KnightAnimInstance.h"
#include "Combat.h"
#include "CombatGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "CombatCharacterStatComponent.h"
#include "CombatCharacterWidget.h"
#include "CombatCharacterSetting.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "CombatAIController.h"
#include "CombatPlayerController.h"
#include "CombatHUDWidget.h"
#include "CombatGameInstance.h"
#include "CombatProjectile.h"
#include "PickUp_Tech.h"
#include "EnemyKnight.h"
//#include "DrawDebugHelpers.h"
#include "PickUp_Bronze.h"
#include "PickUp_Purple.h"
#include "PickUp_WhiteGold.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatPlayerState.h"
#include "GameFramework/Pawn.h"
#include "EnemyStatComponent.h"
#include "EnemyWidget.h"
#include "GameFramework/PlayerState.h"

//////////////////////////////////////////////////////////////////////////
// ACombatCharacter

ACombatCharacter::ACombatCharacter()
{
	Reach = 300.f;	// 픽업 거리

	UseGuardKey = false;
	UseMonarchKey = false;

	EnemyKnockBack = false;

	IsAttacking = false;	// 기본 상태 : 공격 중 X
	IsSprinting = false;	// 기본 상태 : 스프린트 중 X
	OnEquip = false;
	OnTech = false;
	OnPurple = false;

	//WeaponAttack = 10.f;	

	MaxCombo = 4;	// 최대 콤보 공격 수
	AttackEndComboState();

	AttackRange = 100.0f;	// 기본 공격 거리
	AttackRadius = 50.0f;	// 기본 공격 넓이

	FireSlashRange = 200.0f;	// 기본 공격 거리
	IceShoutRange = 500.0f;	// 기본 공격 거리
	IceShoutRadius = 100.f;

	HurricaneRange = 500.0f;	// 기본 공격 거리
	HurricaneRadius = 500.0f;	// 기본 공격 넓이

	DeadTimer = 5.f;

	CurrentStat = CreateDefaultSubobject<UCombatCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	/*
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	StaminaBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("STAMINABARWIDGET"));
	ExpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EXPBARWIDGET"));

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/ThirdPerson/Blueprints/HUDs/HUD_InGame.HUD_InGame_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(400.0f, 30.0f));

		StaminaBarWidget->SetWidgetClass(UI_HUD.Class);
		StaminaBarWidget->SetDrawSize(FVector2D(400.0f, 30.0f));

		ExpBarWidget->SetWidgetClass(UI_HUD.Class);
		ExpBarWidget->SetDrawSize(FVector2D(450.0f, 15.0f));
	}
	*/
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	AIControllerClass = ACombatAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PCharacter"));
}

void ACombatCharacter::SetCharacterState(ECharacterState NewState)
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
			//SetActorHiddenInGame(true);
			//HPBarWidget->SetHiddenInGame(true);
			//SetCanBeDamaged(false);
			CombatPlayerController->GetHUDWidget()->BindCharacterStat(CurrentStat);

			auto CombatPlayerState = Cast<ACombatPlayerState>(GetPlayerState());
			PCHECK(nullptr != CombatPlayerState);
			CurrentStat->SetNewLevel(CombatPlayerState->GetCharacterLevel());
			//CurrentStat->CurrentExp = CombatPlayerState->CurrentExp;
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

ECharacterState ACombatCharacter::GetCharacterState() const
{
	return CurrentState;
}

void ACombatCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	PCHECK(nullptr != AssetLoaded);
	if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);

		SetCharacterState(ECharacterState::READY);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACombatCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACombatCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACombatCharacter::StopSprint);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACombatCharacter::Dash);
	PlayerInputComponent->BindAction("BackDive", IE_Pressed, this, &ACombatCharacter::BackDive);
	
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACombatCharacter::Attack);
	PlayerInputComponent->BindAction("FireSlash", IE_Pressed, this, &ACombatCharacter::FireSlash);
	PlayerInputComponent->BindAction("FireProjectile", IE_Pressed, this, &ACombatCharacter::FireProjectile);
	PlayerInputComponent->BindAction("IceShout", IE_Pressed, this, &ACombatCharacter::IceShout);
	PlayerInputComponent->BindAction("Buff", IE_Pressed, this, &ACombatCharacter::Buff);
	PlayerInputComponent->BindAction("Hurricane", IE_Pressed, this, &ACombatCharacter::Hurricane);

	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ACombatCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("ToggleEquip", IE_Pressed, this, &ACombatCharacter::ToggleEquip);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACombatCharacter::Interact);

	//PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &APickUp::OnPickedUp);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACombatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACombatCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACombatCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACombatCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACombatCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACombatCharacter::TouchStopped);
}

void ACombatCharacter::GetKnockBack()
{
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));

	PAnim->GetIceKnockBack();

	GetWorldTimerManager().SetTimer(KnockBackHandle, this, &ACombatCharacter::GetEnableInput, 1.5f, false);
}

void ACombatCharacter::GetEnableInput()
{
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}

void ACombatCharacter::GetSlow()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	SprintSpeed = 200.f;

	GetWorldTimerManager().SetTimer(SlowHandle, this, &ACombatCharacter::GetNotSlow, 4.f, false);
}

void ACombatCharacter::GetMonarchTornadoSlow()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	SprintSpeed = 300.f;

	GetWorldTimerManager().SetTimer(MonarchSlowHandle, this, &ACombatCharacter::GetNotSlow, 5.f, false);
}

void ACombatCharacter::GetNotSlow()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	SprintSpeed = 1000.f;
}

void ACombatCharacter::GetStun()
{
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));

	PAnim->GetLightStun();

	GetWorldTimerManager().SetTimer(StunHandle, this, &ACombatCharacter::GetEnableInput, 2.f, false);
}

void ACombatCharacter::ToggleInventory()
{
	ACombatGameMode* GameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->GetHUDState() == GameMode->HS_InGame)
	{
		GameMode->ChangeHUDState(GameMode->HS_Inventory);
	}
	else
	{
		GameMode->ChangeHUDState(GameMode->HS_InGame);
	}
}

void ACombatCharacter::ToggleEquip()
{
	ACombatGameMode* GameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->GetHUDState() == GameMode->HS_InGame)
	{
		GameMode->ChangeHUDState(GameMode->HS_Equip);
	}
	else
	{
		GameMode->ChangeHUDState(GameMode->HS_InGame);
	}
}

void ACombatCharacter::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}
}

void ACombatCharacter::CheckForInteractable()
{
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = (GetActorForwardVector() * Reach) - (GetActorUpVector() * Reach) + StartTrace;

	FHitResult HitResult;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	AInteractable* PotentialInteractable = Cast<AInteractable>(HitResult.GetActor());

	if (PotentialInteractable == NULL)
	{
		HelpText = FString("");
		CurrentInteractable = nullptr;

		return;
	}
	else
	{
		CurrentInteractable = PotentialInteractable;
		HelpText = PotentialInteractable->InteractableHelpText;
	}
}

// 점프 시작
void ACombatCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

// 점프 마지막
void ACombatCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

// 달리기
void ACombatCharacter::Sprint()
{
	if (CurrentStat->CurrentStamina > 0)
	{
		IsSprinting = true;
		CurrentStat->ControlSprint(true);
		CurrentStat->SetSprint();
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		StopSprint();
	}
}

// 달리기 멈춤
void ACombatCharacter::StopSprint()
{
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	CurrentStat->ControlSprint(false);
}

void ACombatCharacter::HandleSprint()
{
	if (IsSprinting)
	{
		CurrentStat->DecreaseStamina(1.f);
	}
	else
	{
		CurrentStat->RegenStamina();
	}
}

// 앞 대쉬
void ACombatCharacter::Dash()
{
	if ((!GetCharacterMovement()->IsFalling()) && (CurrentStat->CurrentStamina >= 20.f))	// 점프 상태 제외
	{
		if (DashCoolDown >= 1.f)
		{
			const FVector ForwardDir = this->GetActorRotation().Vector();
			LaunchCharacter(ForwardDir * DashDistance, true, true);

			CurrentStat->CurrentStamina -= 20.f;

			if (DashMontage)
			{
				PlayAnimMontage(DashMontage, 1, NAME_None);
			}

			DashCoolDown = 0.f;
		}

		GetWorld()->GetTimerManager().SetTimer(DashHandle, this, &ACombatCharacter::FillDashCoolDown, 0.4f, true);
	}
}

// 뒷 구르기
void ACombatCharacter::BackDive()
{
	if ((!GetCharacterMovement()->IsFalling()) && (CurrentStat->CurrentStamina >= 20.f) && OnEquip)	// 점프 상태 제외
	{
		if (BackDiveCoolDown >= 1.f)
		{
			AttackCheck();

			const FVector ForwardDir = this->GetActorRotation().Vector();
			LaunchCharacter(ForwardDir * BDDistance, true, true);

			CurrentStat->CurrentStamina -= 20.f;

			if (BDMontage)
			{
				PlayAnimMontage(BDMontage, 1, NAME_None);
			}

			BackDiveCoolDown = 0.f;
		}
		
		GetWorld()->GetTimerManager().SetTimer(BackDiveHandle, this, &ACombatCharacter::FillBackDiveCoolDown, 0.4f, true);
	}
}

// 쿨타임 채우기
void ACombatCharacter::FillDashCoolDown()
{
	DashCoolDown += 0.5f;
}

// 쿨타임 채우기
void ACombatCharacter::FillBackDiveCoolDown()
{
	BackDiveCoolDown += 0.5f;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SprintHandle, this, &ACombatCharacter::HandleSprint, 0.1f, true);

	Inventory.SetNum(10);
	
	CurrentInteractable = nullptr;

	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		CombatPlayerController = Cast<ACombatPlayerController>(GetController());
		PCHECK(nullptr != CombatPlayerController);
	}
	else
	{
		CombatAIController = Cast<ACombatAIController>(GetController());
		PCHECK(nullptr != CombatAIController);
	}

	auto DefaultSetting = GetDefault<UCombatCharacterSetting>();

	if (bIsPlayer)
	{
		auto CombatPlayerState = Cast<ACombatPlayerState>(GetPlayerState());
		PCHECK(nullptr != CombatPlayerState);
		AssetIndex = CombatPlayerState->GetCharacterIndex();
	}
	else
	{
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
		auto CombatGameInstance = Cast<UCombatGameInstance>(GetGameInstance());
		PCHECK(nullptr != CombatGameInstance);
		AssetStreamingHandle = CombatGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ACombatCharacter::OnAssetLoadCompleted));

		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	SetCharacterState(ECharacterState::LOADING);
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractable();

	if (CurrentStat->CurrentStamina <= 0)
	{
		StopSprint();
	}
}

// 애니메이션 관리
void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	PCHECK(nullptr != PAnim);

	PAnim->OnMontageEnded.AddDynamic(this, &ACombatCharacter::OnAttackMontageEnded);

	PAnim->OnNextAttackCheck.AddLambda([this]() -> void {CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			PAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});

	PAnim->OnAttackHitCheck.AddUObject(this, &ACombatCharacter::AttackCheck);

	CurrentStat->OnHPIsZero.AddLambda([this]()->void {PAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, this, &ACombatCharacter::Respawn, DeadTimer, false);
		});

	/*
	PCHECK(nullptr != EnemyStat);
	EnemyStat->OnHPIsZero.AddLambda([this]()->void {
		CombatPlayerController = Cast<ACombatPlayerController>(GetController());
		PCHECK(nullptr != CombatPlayerController);
		CombatPlayerController->EnemyKill(this);
		});
		*/
}

// 기본 공격
void ACombatCharacter::Attack()
{
	if (!GetCharacterMovement()->IsFalling() && OnEquip)	// 점프 상태 제외
	{
		GetCharacterMovement()->DisableMovement();	// 움직임 정지

		if (IsAttacking)	// 기본 공격 중
		{
			PCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
			if (CanNextCombo)	// 다음 콤보로
			{
				IsComboInputOn = true;
			}
		}
		else  // 기본 공격 중 X 
		{
			PCHECK(CurrentCombo == 0);	// 현재 콤보 0
			AttackStartComboState();	// 콤보 공격 시작
			PAnim->ComboAttackMontage();	// 콤보 공격 시작 몽타주
			PAnim->JumpToAttackMontageSection(CurrentCombo);	// 다음 콤보 공격 진행 몽타주
			IsAttacking = true;
		}

		GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
	}
}

// 화염 속성 스킬
void ACombatCharacter::FireSlash()
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 2)
	{
		if (!GetCharacterMovement()->IsFalling() && OnEquip)	// 점프 상태 제외
		{
			if (FireCoolDown >= 5.f)
			{
				GetCharacterMovement()->DisableMovement();	// 움직임 정지

				if (!IsAttacking)
				{
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda([this]()->void {FireSlashCheck(); }), 0.2f, false);	// 공격 실행
					
					PAnim->FireSlash();	// 공격 몽타주
				
					FireCoolDown = 0.f;
				}

				GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
			}

			GetWorld()->GetTimerManager().SetTimer(FillFireHandle, this, &ACombatCharacter::FillFireCoolDown, 0.1f, true);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.2"));
	}
}

void ACombatCharacter::FireProjectile()
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 5)
	{
		if (!GetCharacterMovement()->IsFalling() && OnEquip)	// 점프 상태 제외
		{
			if (FireProjectileCoolDown >= 5.f)
			{
				GetCharacterMovement()->DisableMovement();	// 움직임 정지

				if (!IsAttacking)
				{
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda([this]()->void {FireProjectileCheck(); }), 0.2f, false);	// 공격 실행
					PAnim->FireProjectile();	// 공격 몽타주
					FireProjectileCoolDown = 0.f;
				}

				GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
			}

			GetWorld()->GetTimerManager().SetTimer(FillFireProjectileHandle, this, &ACombatCharacter::FillFireProjectileCoolDown, 0.1f, true);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.5"));
	}
}

// 얼음 속성 스킬
void ACombatCharacter::IceShout()
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 7)
	{
		if (!GetCharacterMovement()->IsFalling() && OnEquip)	// 점프 상태 제외
		{
			if (IceCoolDown >= 5.f)
			{
				GetCharacterMovement()->DisableMovement();	// 움직임 정지

				if (!IsAttacking)
				{
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda([this]()->void {IceShoutCheck(); }), 0.2f, false);	// 공격 실행
					PAnim->IceShout();	// 공격 몽타주
					IceCoolDown = 0.f;
				}

				GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
			}

			GetWorld()->GetTimerManager().SetTimer(FillIceHandle, this, &ACombatCharacter::FillIceCoolDown, 0.1f, true);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.7"));
	}
}

void ACombatCharacter::Buff()
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 7)
	{
		if (!GetCharacterMovement()->IsFalling() && OnEquip)	// 점프 상태 제외
		{
			if (BuffCoolDown >= 60.f)
			{
				GetCharacterMovement()->DisableMovement();	// 움직임 정지

				if (!IsAttacking)
				{
					GetWorld()->GetTimerManager().SetTimer(BuffTimerHandle, FTimerDelegate::CreateLambda([this]()->void {BuffCheck(); }), 0.2f, false);	// 공격 실행
					PAnim->Buff();	// 공격 몽타주
					BuffCoolDown = 0.f;
				}

				GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
			}

			GetWorld()->GetTimerManager().SetTimer(FillBuffHandle, this, &ACombatCharacter::FillBuffCoolDown, 0.1f, true);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.7"));
	}
}

// 궁극 스킬
void ACombatCharacter::Hurricane()
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 10)
	{
		if (!GetCharacterMovement()->IsFalling() && OnEquip)	// 점프 상태 제외
		{
			if (HurricaneCoolDown >= 10.f)
			{
				GetCharacterMovement()->DisableMovement();	// 움직임 정지

				if (!IsAttacking)
				{
					GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACombatCharacter::HurricaneCheck, 0.5f, true);	// 공격 실행
					PAnim->Hurricane();	// 공격 몽타주
					HurricaneCoolDown = 0.f;
					GetWorldTimerManager().SetTimer(HurricaneHandle, this, &ACombatCharacter::HurricaneEnd, 5.f, false);
				}

				GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);	// 움직임 정지 해제
			}

			GetWorld()->GetTimerManager().SetTimer(FillHurricaneHandle, this, &ACombatCharacter::FillHurricaneCoolDown, 0.1f, true);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.10"));
	}
}

void ACombatCharacter::HurricaneEnd()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

// 쿨타임 채우기
void ACombatCharacter::FillFireCoolDown()
{
	FireCoolDown += 0.1f;
}

void ACombatCharacter::FillFireProjectileCoolDown()
{
	FireProjectileCoolDown += 0.1f;
}

// 쿨타임 채우기
void ACombatCharacter::FillIceCoolDown()
{
	IceCoolDown += 0.1f;
}

void ACombatCharacter::FillBuffCoolDown()
{
	BuffCoolDown += 0.1f;
}

// 쿨타임 채우기
void ACombatCharacter::FillHurricaneCoolDown()
{
	HurricaneCoolDown += 0.1f;
}

float ACombatCharacter::TotalAttack()
{
	PCHECK(nullptr != PlayerStatData, 0.0f);

	return PlayerStatData->Attack + WeaponAttack;
}

// 기본 공격 정보
void ACombatCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 100.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	// 공격 범위 표시
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5 + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;

		if (EnemyKnockBack == false)
		{
			HitResult.GetActor()->TakeDamage(CurrentStat->GetAttack() * 1 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
		}
		else
		{
			HitResult.GetActor()->TakeDamage(CurrentStat->GetAttack() * 1.5 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
		}
	}
}

// 화염 속성 스킬 정보
void ACombatCharacter::FireSlashCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,	// 공격 거리
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),	// 공격 범위
		Params);

	// 공격 범위 표시
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * FireSlashRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FireSlashRange * 0.5 + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;	// 1초 동안

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	*/
//#endif
	if (bResult)
	{
		FDamageEvent DamageEvent;

		if (EnemyKnockBack == false)
		{
			HitResult.GetActor()->TakeDamage(CurrentStat->GetAttack() * 2.2 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
		}
		else
		{
			HitResult.GetActor()->TakeDamage(CurrentStat->GetAttack() * 3.2 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
		}
	}
}

void ACombatCharacter::FireProjectileCheck()
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(50.f, 10.f, 70.f));	// 공격체 스폰 위치
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<ACombatProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	}
}

// 얼음 속성 스킬 정보
void ACombatCharacter::IceShoutCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 500.0f,	// 공격 거리
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(100.f),	// 공격 범위
		Params);

	// 공격 범위 표시
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * IceShoutRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = IceShoutRange * 0.5 + IceShoutRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, IceShoutRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif
*/
	if (bResult)
	{
		FDamageEvent DamageEvent;

		if (EnemyKnockBack == false)
		{
			HitResult.GetActor()->TakeDamage(CurrentStat->GetAttack() * 1.5 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
		}
		else
		{
			HitResult.GetActor()->TakeDamage(CurrentStat->GetAttack() * 2 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
		}
	}
}

void ACombatCharacter::BuffCheck()
{
	BuffAttack = 10.f;

	GetWorldTimerManager().SetTimer(BuffTimerHandle, this, &ACombatCharacter::BuffEndCheck, 60.f, true);
}

void ACombatCharacter::BuffEndCheck()
{
	BuffAttack = 0.f;
}

// 궁극 스킬 정보
void ACombatCharacter::HurricaneCheck()
{
	TArray<FOverlapResult> HitResults;
	FVector Center = this->GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(400.0f),
		Params);

	// 공격 범위 표시
	/*
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector();
	//FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = HurricaneRadius * 0.5;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugSphere(GetWorld(), Center, HurricaneRadius, 16, DrawColor, false, DebugLifeTime);

#endif
*/
	if (bResult)
	{
		for (auto & hit : HitResults )
		{
			FDamageEvent DamageEvent;

			if (EnemyKnockBack == false)
			{
				hit.GetActor()->TakeDamage(CurrentStat->GetAttack() * 1.5 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
			}
			else
			{
				hit.GetActor()->TakeDamage(CurrentStat->GetAttack() * 2 + WeaponAttack + DrugAttack + BuffAttack, DamageEvent, GetController(), this);	// 대미지
			}
		}
	}
}

// 대미지
float ACombatCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	PAnim->HitReact();	// 피격 시 몽타주

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ACombatCharacter::UpdateGold(int32 Amount)
{
	Gold = Gold + Amount;
}

bool ACombatCharacter::AddItemToInventory(APickUp* Item)
{
	if (Item != NULL)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);

		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FULL"));
			return false;
		}
	}
	else return false;
}

UTexture2D* ACombatCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickUpThumbnail;
	}
	else return nullptr;
}

FString ACombatCharacter::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->ItemName;
	}
	return FString("None");
}

FString ACombatCharacter::GetEquipAtInventorySlot(int32 Slot)
{
	
	return FString();
}

FString ACombatCharacter::UseItemInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		Inventory[Slot]->Use_Implementation();
		Inventory[Slot] = NULL;
	}

	return FString();
}

UTexture2D* ACombatCharacter::BuyShopSlot(int32 Slot)
{
	if (Shop[Slot] != NULL)
	{
		if (Gold >= 100)
		{
			{
				Shop[Slot]->Buy_Implementation();

				if (Inventory[Slot] != NULL)
				{
					return Inventory[Slot]->PickUpThumbnail;
				}
			}
		}
	}

	return nullptr;
}

int32 ACombatCharacter::GetExp() const
{
	//PCHECK(nullptr != EnemyStat);

	return EnemyStat->GetDropExp();
}

void ACombatCharacter::Respawn()
{
	//SetActorEnableCollision(true);
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
	//GetMesh()->SetHiddenInGame(true);
	//SetActorLocation(FVector(8550.f, 4365.f, -1330.f));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FrozenCove"));
}

// Tech 무기 장착
void ACombatCharacter::SetTech(APickUp* SetWeapon)
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 1)
	{
		PCHECK(nullptr != SetWeapon);
	
		if (nullptr != CurWeapon)
		{
			CurWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurWeapon->InteractableMesh->SetVisibility(false);
			CurWeapon->Destroy();
			CurWeapon = nullptr;
		}

		FName WeaponSocket(TEXT("Tech"));

		SetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		SetWeapon->SetOwner(this);
		CurWeapon = SetWeapon;
		OnEquip = true;

		WeaponAttack = 10.f;	// 무기 공격력
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.1"));
	}
}

void ACombatCharacter::SetPurple(APickUp* SetWeapon)
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 5)
	{
		PCHECK(nullptr != SetWeapon);

		if (nullptr != CurWeapon)
		{
			CurWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurWeapon->InteractableMesh->SetVisibility(false);
			CurWeapon->Destroy();
			CurWeapon = nullptr;
		}

		FName WeaponSocket(TEXT("Purple"));

		SetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		SetWeapon->SetOwner(this);
		CurWeapon = SetWeapon;
		OnEquip = true;

		WeaponAttack = 20.f;	// 무기 공격력
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.5"));
	}
}

// Bronze 무기 장착
void ACombatCharacter::SetBronze(APickUp* SetWeapon)
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 10)
	{
		PCHECK(nullptr != SetWeapon);
	
		if (nullptr != CurWeapon)
		{
			CurWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurWeapon->InteractableMesh->SetVisibility(false);
			CurWeapon->Destroy();
			CurWeapon = nullptr;
		}

		FName WeaponSocket(TEXT("Bronze"));

		SetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		SetWeapon->SetOwner(this);
		CurWeapon = SetWeapon;
		OnEquip = true;

		WeaponAttack = 30.f;	// 무기 공격력
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.10"));
	}
}

void ACombatCharacter::SetWhiteGold(APickUp* SetWeapon)
{
	PCHECK(nullptr != CurrentStat);
	if (CurrentStat->Level >= 15)
	{
		PCHECK(nullptr != SetWeapon);

		if (nullptr != CurWeapon)
		{
			CurWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurWeapon->InteractableMesh->SetVisibility(false);
			CurWeapon->Destroy();
			CurWeapon = nullptr;
		}

		FName WeaponSocket(TEXT("WhiteGold"));

		SetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		SetWeapon->SetOwner(this);
		CurWeapon = SetWeapon;
		OnEquip = true;

		WeaponAttack = 40.f;	// 무기 공격력
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lv.15"));
	}
}

bool ACombatCharacter::CanSetWeapon()
{
	return true;
}

// 콤보 공격 이어가지 못했을때
void ACombatCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	PCHECK(IsAttacking);
	PCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

// 콤보 공격 상태 시작
void ACombatCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	PCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// 콤보 공격 상태 종료
void ACombatCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

// 작은 포션 사용
void ACombatCharacter::UseSmallPotion()
{
	PCHECK(nullptr != CurrentStat);

	CurrentStat->CurrentHP += 500.f;
}

// 큰 포션 사용
void ACombatCharacter::UseBigPotion()
{
	auto CombatGameInstance = Cast<UCombatGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PCHECK(nullptr != CombatGameInstance);
	PlayerStatData = CombatGameInstance->GetCombatCharacterData(CurrentStat->Level);
	PCHECK(nullptr != CurrentStat);
	if (nullptr != PlayerStatData)
	{
		CurrentStat->CurrentHP = PlayerStatData->MaxHP;
	}
}

// 영약 사용
void ACombatCharacter::UseDrug()
{
	DrugAttack = 10.f;

	GetWorld()->GetTimerManager().SetTimer(DrugHandle, this, &ACombatCharacter::UseDrugEnd, 60.f, false);
}

// 영약 사용 끝
void ACombatCharacter::UseDrugEnd()
{
	DrugAttack = 0.f;
}

void ACombatCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACombatCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACombatCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACombatCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


// Enemy


