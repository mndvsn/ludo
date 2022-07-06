// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoGameState.h"
#include "Net/UnrealNetwork.h"

#include "LudoGameModeBase.h"
#include "Game/GamerState.h"


ALudoGameState::ALudoGameState()
{
	EventComponent = CreateDefaultSubobject<ULudoEventComponent>("EventComponent");
}

void ALudoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALudoGameState, CurrentPlayerIndex);
}

const AGamerState* ALudoGameState::GetGamerStateForIndex(int8 PlayerIndex) const
{
	const TObjectPtr<APlayerState>* PlayerStatePtr = PlayerArray.FindByPredicate([PlayerIndex](const TObjectPtr<APlayerState>& PlayerState)
	{
		auto GamerState = Cast<AGamerState>(PlayerState);
		return (GamerState->GetPlayerIndex() == PlayerIndex);
	});

	if (!PlayerStatePtr) return nullptr;

	return CastChecked<AGamerState>(PlayerStatePtr->Get());
}

const class AGamerState* ALudoGameState::GetGamerStateInTurn() const
{
	return GetGamerStateForIndex(CurrentPlayerIndex);
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
