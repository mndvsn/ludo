// Copyright Martin Furuberg. All Rights Reserved.


#include "CreateGameWidget.h"

#include <Components/Button.h>
#include <Components/Slider.h>
#include <Components/EditableTextBox.h>
#include <Components/CheckBox.h>

#include "Game/LudoGameInstance.h"
#include "Game/LudoGameState.h"


UCreateGameWidget::UCreateGameWidget(const FObjectInitializer& ObjectInitializer) : UMainMenuSubWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UCreateGameWidget::NativeOnInitialized()
{
	if (ButtonBack == nullptr || ButtonCreateGame == nullptr) return;

	ButtonCreateGame->OnReleased.AddDynamic(this, &UCreateGameWidget::ButtonCreateGameReleased);
	ButtonBack->OnReleased.AddDynamic(this, &UCreateGameWidget::ButtonBackReleased);

	// Set default seed from timestamp
	const int64 Timestamp = FDateTime::UtcNow().ToUnixTimestamp();
	FNumberFormattingOptions Options = FNumberFormattingOptions::DefaultNoGrouping();
	Options.MaximumFractionalDigits = 0;
	TextBoxSeed->SetText(FText::AsNumber(Timestamp, &Options));

	const FGameSettings DefaultSettings = FGameSettings();
	CheckBoxKnock->SetIsChecked(DefaultSettings.bKnockPieces);
	CheckBoxKnockMultiple->SetIsChecked(DefaultSettings.bKnockMultiple);
	CheckBoxMoveHighRoll->SetIsChecked(DefaultSettings.bMoveOnHighEntryRoll);
}

void UCreateGameWidget::ButtonCreateGameReleased()
{
	if (SliderPlayersCPU == nullptr) return;
	const uint32 Seed = FCString::Atoi(*TextBoxSeed->GetText().ToString());
	const uint8 NumPlayersCPU = static_cast<uint8>(SliderPlayersCPU->GetValue());
	const char NumPlayers = NumPlayersCPU + 1;
	const bool bKnockPieces = CheckBoxKnock->IsChecked();
	const bool bKnockMultiple = CheckBoxKnockMultiple->IsChecked();
	const bool bMoveOnHighEntryRoll = CheckBoxMoveHighRoll->IsChecked();

	if (const TObjectPtr<ULudoGameInstance> GameInstance = GetGameInstance<ULudoGameInstance>())
	{
		GameInstance->CreateGame(Seed, NumPlayers, NumPlayersCPU, bKnockPieces, bKnockMultiple, bMoveOnHighEntryRoll);
	}
}

void UCreateGameWidget::ButtonBackReleased()
{
	if (GetMenuInterface() == nullptr) return;

	GetMenuInterface()->ReturnToParentMenu();
}
