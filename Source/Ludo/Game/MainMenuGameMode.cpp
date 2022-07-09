// Copyright Martin Furuberg. All Rights Reserved.


#include "MainMenuGameMode.h"
#include "GameFramework/DefaultPawn.h"

#include "UI/MenuHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	DefaultPawnClass = ADefaultPawn::StaticClass();
	HUDClass = AMenuHUD::StaticClass();

	//bUseSeamlessTravel = true;
}
