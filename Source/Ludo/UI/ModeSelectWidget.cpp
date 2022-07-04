// Copyright Martin Furuberg. All Rights Reserved.


#include "ModeSelectWidget.h"
#include "Components/Button.h"


UModeSelectWidget::UModeSelectWidget(const FObjectInitializer& ObjectInitializer) : UMainMenuSubWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UModeSelectWidget::NativeOnInitialized()
{
	ButtonGameCPU->OnReleased.AddDynamic(this, &UModeSelectWidget::ButtonGameCPUReleased);
}

void UModeSelectWidget::ButtonGameCPUReleased()
{
	if (GetMenuInterface() == nullptr) return;

	GetMenuInterface()->NavigateMenu(EMainMenu::MM_ModeSelect);
}
