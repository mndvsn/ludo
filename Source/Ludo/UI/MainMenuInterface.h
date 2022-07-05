// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainMenuInterface.generated.h"

// Submenus in main menu
UENUM(BlueprintType)
enum class EMainMenu : uint8 {
	MM_None  UMETA(DisplayName = "None"),
	MM_ModeSelect  UMETA(DisplayName = "ModeSelect"),
	MM_CreateGame  UMETA(DisplayName = "CreateGame"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LUDO_API IMainMenuInterface
{
	GENERATED_BODY()

public:

	virtual void ReturnToParentMenu() = 0;

	virtual void NavigateMenu(EMainMenu ToMenu) = 0;
};
