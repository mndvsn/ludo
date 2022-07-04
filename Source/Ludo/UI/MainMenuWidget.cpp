// Copyright Martin Furuberg. All Rights Reserved.


#include "MainMenuWidget.h"

#include "Components/NamedSlot.h"
#include "Components/Button.h"

#include "MainMenuSubWidget.h"
#include "ModeSelectWidget.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
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
	if (DefaultMainContent_Class == nullptr) return;
	RootMenu = CreateWidget<UMainMenuSubWidget>(this, DefaultMainContent_Class);
	RootMenu->SetMenuInterface(this);

	NavigateMenu(RootMenu->GetEnum());
}

void UMainMenuWidget::NavigateMenu(EMainMenu ToMenu)
{
	UE_LOG(LogTemp, Warning, TEXT("Navigate"));

	EMainMenu CurrentMenu = (MenuStack.Num() > 0) ? MenuStack.Top() : EMainMenu::MM_None;
	if (CurrentMenu == ToMenu)
	{
		UE_LOG(LogTemp, Warning, TEXT("Navigation failed: Already at target menu"));
		return;
	}

	// Add menu to navigation history
	MenuStack.Add(ToMenu);

	switch (ToMenu)
	{
	default:
	case EMainMenu::MM_ModeSelect:		
		SetMainContent(RootMenu);
		break;
	}
}

void UMainMenuWidget::ReturnToParentMenu()
{
	if (MenuStack.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Navigation failed: Already at root menu"));
		return;
	}

	EMainMenu CurrentMenu = MenuStack.Top();
	EMainMenu LastMenu = MenuStack.Last(1);

	NavigateMenu(LastMenu);
}

void UMainMenuWidget::SetMainContent(UWidget* Content)
{
	SlotMain->SetContent(Content);
}

void UMainMenuWidget::ButtonExitGameReleased()
{
	GetOwningPlayer()->ConsoleCommand("quit");
}
