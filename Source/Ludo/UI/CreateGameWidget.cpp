// Copyright Martin Furuberg. All Rights Reserved.


#include "CreateGameWidget.h"

#include <Components/Button.h>
#include <Components/Slider.h>
#include <Components/EditableTextBox.h>

#include "Game/LudoGameInstance.h"


UCreateGameWidget::UCreateGameWidget(const FObjectInitializer& ObjectInitializer) : UMainMenuSubWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UCreateGameWidget::NativeOnInitialized()
{
	if (ButtonBack == nullptr || ButtonCreateGame == nullptr) return;

	ButtonCreateGame->OnReleased.AddDynamic(this, &UCreateGameWidget::ButtonCreateGameReleased);
	ButtonBack->OnReleased.AddDynamic(this, &UCreateGameWidget::ButtonBackReleased);

	const int64 Timestamp = FDateTime::UtcNow().ToUnixTimestamp();
	FNumberFormattingOptions Options = FNumberFormattingOptions::DefaultNoGrouping();
	Options.MaximumFractionalDigits = 0;
	TextBoxSeed->SetText(FText::AsNumber(Timestamp, &Options));
}

void UCreateGameWidget::ButtonCreateGameReleased()
{
	if (SliderPlayersCPU == nullptr) return;
	const uint8 NumPlayersCPU = static_cast<uint8>(SliderPlayersCPU->GetValue());
	const uint32 Seed = FCString::Atoi(*TextBoxSeed->GetText().ToString());
	const char NumPlayers = NumPlayersCPU + 1;

	if (const TObjectPtr<ULudoGameInstance> GameInstance = GetGameInstance<ULudoGameInstance>())
	{
		GameInstance->CreateGame(Seed, NumPlayers, NumPlayersCPU);
	}
}

void UCreateGameWidget::ButtonBackReleased()
{
	if (GetMenuInterface() == nullptr) return;

	GetMenuInterface()->ReturnToParentMenu();
}
