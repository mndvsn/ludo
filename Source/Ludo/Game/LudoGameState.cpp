// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoGameState.h"
#include "Net/UnrealNetwork.h"

#include "LudoGameModeBase.h"


ALudoGameState::ALudoGameState()
{
	EventComponent = CreateDefaultSubobject<ULudoEventComponent>("EventComponent");
}

void ALudoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALudoGameState, CurrentPlayerIndex);
}

bool ALudoGameState::IsPlayerTurn(APlayerController* Player)
{
	return true;
}

void ALudoGameState::AdvanceTurn()
{
	if (!HasAuthority()) return;

	UE_LOG(LogTemp, Warning, TEXT("-- ADVANCE TURN --"));

	uint8 NumPlayers = PlayerArray.Num();

	if (NumPlayers == ++CurrentPlayerIndex)
	{
		CurrentPlayerIndex = 0;
	}

	GetEvents()->OnTurnChange.Broadcast(CurrentPlayerIndex);
}

void ALudoGameState::OnRep_CurrentPlayerIndex()
{
	GetEvents()->OnTurnChange.Broadcast(CurrentPlayerIndex);
}
