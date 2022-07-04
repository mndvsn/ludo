// Copyright Martin Furuberg. All Rights Reserved.


#include "MainMenuWidget.h"

#include "Components/NamedSlot.h"
#include "Components/Button.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UMainMenuWidget::SetMainContent(UWidget* Content)
{
	SlotMain->SetContent(Content);
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
	RootMenu = CreateWidget(this, DefaultMainContent_Class);
	
	SetMainContent(RootMenu);
}

void UMainMenuWidget::ButtonExitGameReleased()
{
	GetOwningPlayer()->ConsoleCommand("quit");
}
