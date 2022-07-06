// Copyright Martin Furuberg. All Rights Reserved.


#include "GameHUD.h"

#include "GameOverlayWidget.h"
#include "Game/LudoGameInstance.h"


AGameHUD::AGameHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> GameWidget_BPClass(TEXT("/Game/UI/Game/W_Game"));
	if (GameWidget_BPClass.Class) {
		GameWidget_Class = GameWidget_BPClass.Class;
	}
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
	if (!IsValid(GameWidget_Class)) return;

	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (!IsValid(PlayerController)) return;

	GameWidget = CreateWidget<UGameOverlayWidget>(PlayerController, GameWidget_Class);
	GameWidget->AddToViewport();
}

void AGameHUD::ShowInGameMenu()
{
	ULudoGameInstance* GameInstance = GetGameInstance<ULudoGameInstance>();
	if (GameInstance == nullptr) return;
	GameInstance->EndGame();
}
