// Copyright Martin Furuberg. All Rights Reserved.


#include "GameHUD.h"

#include "GameOverlayWidget.h"
#include "Game/LudoGameInstance.h"


AGameHUD::AGameHUD()
{
	GameWidgetClass = UGameOverlayWidget::StaticClass();
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	ConstructHUD();
}

void AGameHUD::Show()
{
	// show overlays
}

void AGameHUD::Hide()
{
	// hide overlays
}

void AGameHUD::ConstructHUD()
{
	if (!IsValid(GameWidgetClass)) return;

	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (!IsValid(PlayerController)) return;

	GameWidget = CreateWidget<UGameOverlayWidget>(PlayerController, GameWidgetClass);
	GameWidget->AddToViewport();
}

void AGameHUD::ShowInGameMenu()
{
	ULudoGameInstance* GameInstance = GetGameInstance<ULudoGameInstance>();
	if (GameInstance == nullptr) return;
	GameInstance->EndGame();
}
