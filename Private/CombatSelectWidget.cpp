// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSelectWidget.h"
#include "CombatGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "CombatSaveGame.h"
#include "CombatPlayerState.h"
#include "Combat.h"
#include "CombatCharacterSetting.h"

void UCombatSelectWidget::NextCharacter(bool bForward)
{
	bForward ? ++CurrentIndex : --CurrentIndex;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UCombatCharacterSetting>();
	//auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto CombatGameInstance = GetWorld()->GetGameInstance<UCombatGameInstance>();
	PCHECK(nullptr != CombatGameInstance);
	PCHECK(TargetComponent.IsValid());

	//USkeletalMesh* Asset = CombatGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	//if (nullptr != Asset)
	//{
		//TargetComponent->SetSkeletalMesh(Asset);
	//}
}

void UCombatSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UCombatCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	PCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	PCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	PCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	PCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UCombatSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UCombatSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UCombatSelectWidget::OnConfirmClicked);
}

void UCombatSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UCombatSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UCombatSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	UCombatSaveGame* NewPlayerData = NewObject<UCombatSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 15;
	NewPlayerData->CurrentExp = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto CombatPlayerState = GetDefault<ACombatPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, CombatPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StoryStart"));
	}
}
