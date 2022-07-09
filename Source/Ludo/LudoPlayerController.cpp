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

	PlayerEventComponent = CreateDefaultSubobject<ULudoPlayerEventComponent>("PlayerEventComponent");
}

int8 ALudoPlayerController::GetPlayerIndex() const
{
	return GetPlayerState<AGamerState>()->GetPlayerIndex();
}

void ALudoPlayerController::Client_StartTurn_Implementation()
{
	bInTurn = true;

	GetEvents()->OnPlayerTurn.Broadcast(true);
}

void ALudoPlayerController::Client_EndTurn_Implementation()
{
	bInTurn = false;

	GetEvents()->OnPlayerTurn.Broadcast(false);
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
