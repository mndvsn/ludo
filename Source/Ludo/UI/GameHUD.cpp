// Copyright Martin Furuberg. All Rights Reserved.


#include "GameHUD.h"

#include "Game/LudoGameInstance.h"
#include "UI/GameOverlayWidget.h"
#include "UI/GameEndWidget.h"
#include "Actors/PlayerSlot.h"


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
	if (GameWidgetClass)
	{
		GameWidget = CreateWidget<UGameOverlayWidget>(GetOwner<APlayerController>(), GameWidgetClass);
		GameWidget->AddToViewport();
	}
	OnFinishConstructHUD();
}

void AGameHUD::OnFinishConstructHUD()
{
	OnGameHUDReady.ExecuteIfBound();
}

void AGameHUD::ShowInGameMenu()
{
	ULudoGameInstance* GameInstance = GetGameInstance<ULudoGameInstance>();
	if (GameInstance == nullptr) return;
	GameInstance->EndGame();
}

void AGameHUD::ShowEndScreen(APlayerSlot* WinnerSlot)
{
	if (GameEndWidgetClass)
	{
		EndScreenWidget = CreateWidget<UGameEndWidget>(GetOwner<APlayerController>(), GameEndWidgetClass);
		EndScreenWidget->SetValues(WinnerSlot);
		EndScreenWidget->AddToViewport();
	}
}
