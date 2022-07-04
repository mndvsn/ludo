// Copyright Martin Furuberg. All Rights Reserved.


#include "MainMenuSubWidget.h"

IMainMenuInterface* UMainMenuSubWidget::GetMenuInterface()
{
	return MenuInterface;
}

void UMainMenuSubWidget::SetMenuInterface(IMainMenuInterface* Interface)
{
	if (Interface != nullptr)
	{
		MenuInterface = Interface;
	}
}
