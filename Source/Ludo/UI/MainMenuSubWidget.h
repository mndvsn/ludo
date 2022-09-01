// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "UI/MainMenuInterface.h"
#include "MainMenuSubWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LUDO_API UMainMenuSubWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	IMainMenuInterface* GetMenuInterface();
	void SetMenuInterface(IMainMenuInterface* Interface);

	virtual EMainMenu GetEnum() { return EMainMenu::MM_None; };

private:
	IMainMenuInterface* MenuInterface;
};
