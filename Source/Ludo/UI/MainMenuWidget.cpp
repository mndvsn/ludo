// Copyright Martin Furuberg. All Rights Reserved.


#include "MainMenuWidget.h"

#include "Components/Button.h"


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
}

void UMainMenuWidget::ButtonExitGameReleased()
{
	GetOwningPlayer()->ConsoleCommand("quit");
}
