// Copyright Martin Furuberg. All Rights Reserved.


#include "MainMenuWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/NamedSlot.h"
#include "Components/Button.h"

#include "LudoLog.h"
#include "MainMenuSubWidget.h"
#include "ModeSelectWidget.h"
#include "CreateGameWidget.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();

	MainContentClass = UModeSelectWidget::StaticClass();
	CreateGameWidgetClass = UCreateGameWidget::StaticClass();
}

void UMainMenuWidget::NativeOnInitialized()
{
	auto Button = GetFooter()->GetWidgetFromName("ButtonExit");
	ButtonExitGame = Cast<UButton>(Button);
	if (ButtonExitGame != nullptr)
	{
		ButtonExitGame->OnReleased.AddDynamic(this, &UMainMenuWidget::ButtonExitGameReleased);
	}

	// Create root main menu content
	if (MainContentClass == nullptr) return;
	RootMenu = CreateWidget<UMainMenuSubWidget>(this, MainContentClass);
	RootMenu->SetMenuInterface(this);

	NavigateMenu(RootMenu->GetEnum());
}

void UMainMenuWidget::NavigateMenu(EMainMenu ToMenu)
{
	UE_LOG(LogLudo, Warning, TEXT("Navigate"));

	EMainMenu CurrentMenu = (MenuStack.Num() > 0) ? MenuStack.Top() : EMainMenu::MM_None;
	if (CurrentMenu == ToMenu)
	{
		UE_LOG(LogLudo, Warning, TEXT("Navigation failed: Already at target menu"));
		return;
	}

	switch (ToMenu)
	{
	case EMainMenu::MM_ModeSelect:
	{
		RootMenu->SetVisibility(ESlateVisibility::Visible);
		RootMenu->SetIsEnabled(true);
		SetMainContent(RootMenu);
		break;
	}
	case EMainMenu::MM_CreateGame:
	{
		if (CreateGameWidgetClass == nullptr) return;
		UCreateGameWidget* CreateGameWidget = CreateWidget<UCreateGameWidget>(this, CreateGameWidgetClass);
		CreateGameWidget->SetMenuInterface(this);

		SetMainContent(CreateGameWidget);
		break;
	}
	case EMainMenu::MM_None:
	default:
		break;
	}

	// Add menu to navigation history
	MenuStack.Add(ToMenu);
}

void UMainMenuWidget::ReturnToParentMenu()
{
	if (MenuStack.Num() < 2)
	{
		UE_LOG(LogLudo, Warning, TEXT("Navigation failed: Already at root menu"));
		return;
	}

	EMainMenu CurrentMenu = MenuStack.Top();
	EMainMenu LastMenu = MenuStack.Last(1);

	NavigateMenu(LastMenu);
}

void UMainMenuWidget::SetMainContent(UWidget* Content)
{
	if (Content == nullptr) return;

	UWidget* CurrentContent = SlotMain->GetContent();
	if (CurrentContent != nullptr)
	{
		if (CurrentContent == RootMenu)
		{
			CurrentContent->SetVisibility(ESlateVisibility::Collapsed);
			CurrentContent->SetIsEnabled(false);
		}
		else
		{
			CurrentContent->RemoveFromParent();
		}
	}

	SlotMain->SetContent(Content);
}

void UMainMenuWidget::ButtonExitGameReleased()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}
