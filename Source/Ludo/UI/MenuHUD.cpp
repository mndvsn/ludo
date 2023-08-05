// Copyright Martin Furuberg. All Rights Reserved.


#include "MenuHUD.h"

#include <Blueprint/UserWidget.h>

#include "UI/MainMenuWidget.h"


AMenuHUD::AMenuHUD()
{
	MenuClass = UMainMenuWidget::StaticClass();
}

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	ConstructMenu();
	ShowMenu();
}

void AMenuHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HideMenu();
}

void AMenuHUD::ConstructMenu()
{
	if (MenuClass == nullptr) return;

	APlayerController *PlayerController = GetOwner<APlayerController>();

	Menu = CreateWidget(PlayerController, MenuClass);
}

void AMenuHUD::ShowMenu()
{
	APlayerController *PlayerController = GetOwner<APlayerController>();
	if (PlayerController == nullptr) return;
	if (!IsValid(Menu)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);

	Menu->AddToViewport();
}

void AMenuHUD::HideMenu()
{
	if (!IsValid(Menu)) return;

	Menu->RemoveFromParent();

	if (APlayerController* PlayerController = GetOwner<APlayerController>())
	{
		const FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
}
