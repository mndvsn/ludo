// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoAIController.h"
#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
#include "Game/GamerState.h"
#include "Actors/Gamer.h"


ALudoAIController::ALudoAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsPlayerState = true;
}


void ALudoAIController::Client_StartTurn()
{
	bInTurn = true;

	UE_LOG(LogLudo, Verbose, TEXT("Client_StartTurn (AI)"));

	Process();
}

void ALudoAIController::Client_EndTurn()
{
	bInTurn = false;

	UE_LOG(LogLudo, Verbose, TEXT("Client_EndTurn (AI)"));
}

void ALudoAIController::Server_RequestEndTurn()
{
	if (ALudoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>())
	{
		GameMode->NextTurn();
	}
}

void ALudoAIController::ThrowDie()
{
	//TODO: Check if this player is actually in turn

	TObjectPtr<AGamer> Gamer = GetGamer();
	Gamer->Server_ThrowDie();

	Server_RequestEndTurn();
}

TObjectPtr<AGamer> ALudoAIController::GetGamer()
{
	return GetPawn<AGamer>();
}

TObjectPtr<AGamerState> ALudoAIController::GetGamerState()
{
	return GetPlayerState<AGamerState>();
}

void ALudoAIController::Process()
{
	float RandomDelay = FMath::RandRange(0.5, 2.5);
	GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &ALudoAIController::OnWaited, RandomDelay, false);
}

void ALudoAIController::OnWaited()
{
	ThrowDie();
}
