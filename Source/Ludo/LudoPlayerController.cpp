// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoPlayerController.h"
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

void ALudoPlayerController::Server_RequestEndTurn_Implementation()
{
	ALudoGameState* State = GetWorld()->GetGameState<ALudoGameState>();
	if (State == nullptr) return;

	State->AdvanceTurn();
}

bool ALudoPlayerController::Server_RequestEndTurn_Validate()
{
	return true;
}
