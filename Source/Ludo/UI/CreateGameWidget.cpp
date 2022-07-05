// Copyright Martin Furuberg. All Rights Reserved.


#include "CreateGameWidget.h"
#include "Game/LudoGameInstance.h"

#include "Components/Button.h"


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
	if (GetMenuInterface() == nullptr) return;

	ULudoGameInstance* GameInstance = GetGameInstance<ULudoGameInstance>();
	if (GameInstance == nullptr) return;

	GameInstance->CreateGame();
}

void UCreateGameWidget::ButtonBackReleased()
{
	if (GetMenuInterface() == nullptr) return;

	GetMenuInterface()->ReturnToParentMenu();
}
