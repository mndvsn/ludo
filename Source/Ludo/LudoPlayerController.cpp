// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoPlayerController.h"
#include "Game/LudoGameState.h"


ALudoPlayerController::ALudoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	ClickEventKeys.Add(EKeys::RightMouseButton);
	//bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ALudoPlayerController::SetPlayerIndex(int val)
{
	PlayerIndex = val;
	UE_LOG(LogTemp, Warning, TEXT("Tilldelad player index: %d"), val);
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
