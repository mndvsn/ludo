// Copyright Martin Furuberg. All Rights Reserved.


#include "MenuHUD.h"
#include "Blueprint/UserWidget.h"


AMenuHUD::AMenuHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> Menu_BPClass(TEXT("/Game/UI/W_MainMenu"));
	if (Menu_BPClass.Class) {
		MenuClass = Menu_BPClass.Class;
	}
}

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();
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
	if (!MenuIsValid()) return;

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
	if (!MenuIsValid()) return;

	Menu->RemoveFromParent();

	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (PlayerController == nullptr) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

bool AMenuHUD::MenuIsValid()
{
	return Menu != nullptr;
}
