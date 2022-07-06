// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoPlayerController.h"
#include "Game/LudoGameModeBase.h"
#include "Game/LudoGameState.h"
#include "Game/GamerState.h"


ALudoPlayerController::ALudoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	ClickEventKeys.Add(EKeys::RightMouseButton);
	//bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

int8 ALudoPlayerController::GetPlayerIndex() const
{
	return GetPlayerState<AGamerState>()->GetPlayerIndex();
}

void ALudoPlayerController::Client_StartTurn_Implementation()
{
	InTurn = true;

	// broadcast event
}

void ALudoPlayerController::Client_EndTurn_Implementation()
{
	InTurn = false;

	// broadcast event
}

void ALudoPlayerController::Server_RequestEndTurn_Implementation()
{
	//TODO: Check if this player is actually in turn

	if (ALudoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>())
	{
		GameMode->NextTurn();
	}
}

bool ALudoPlayerController::Server_RequestEndTurn_Validate()
{
	return true;
}
