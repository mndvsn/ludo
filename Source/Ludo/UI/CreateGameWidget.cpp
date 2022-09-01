// Copyright Martin Furuberg. All Rights Reserved.


#include "CreateGameWidget.h"

#include <Components/Button.h>
#include <Components/Slider.h>

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
}

void UCreateGameWidget::ButtonCreateGameReleased()
{
	if (SliderPlayersCPU == nullptr) return;
	uint8 NumPlayersCPU = static_cast<uint8>(SliderPlayersCPU->GetValue());

	ULudoGameInstance* GameInstance = GetGameInstance<ULudoGameInstance>();
	if (GameInstance == nullptr) return;

	GameInstance->CreateGameCPU(NumPlayersCPU + 1, NumPlayersCPU);
}

void UCreateGameWidget::ButtonBackReleased()
{
	if (GetMenuInterface() == nullptr) return;

	GetMenuInterface()->ReturnToParentMenu();
}
