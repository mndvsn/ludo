// Copyright Martin Furuberg. All Rights Reserved.


#include "MenuHUD.h"
#include "Blueprint/UserWidget.h"


AMenuHUD::AMenuHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> Menu_BPClass(TEXT("/Game/UI/MainMenu/W_MainMenu"));
	if (Menu_BPClass.Class) {
		MenuClass = Menu_BPClass.Class;
	}
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

	Menu->bIsFocusable = true;

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
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
}
