// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoAIController.h"
#include "Game/LudoGameModeBase.h"
#include "Game/GamerState.h"


ALudoAIController::ALudoAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsPlayerState = true;
}


void ALudoAIController::Client_StartTurn()
{
	bInTurn = true;

	UE_LOG(LogLudoGM, Verbose, TEXT("Client_StartTurn (AI)"));

	Server_RequestEndTurn();
}

void ALudoAIController::Client_EndTurn()
{
	bInTurn = false;

	UE_LOG(LogLudoGM, Verbose, TEXT("Client_EndTurn (AI)"));
}

void ALudoAIController::Server_RequestEndTurn()
{
	//TODO: Check if this player is actually in turn

	if (ALudoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>())
	{
		GameMode->NextTurn();
	}
}

AGamerState* ALudoAIController::GetGamerState()
{
	return GetPlayerState<AGamerState>();
}
