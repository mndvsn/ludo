// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoGameState.h"
#include "Net/UnrealNetwork.h"

#include "LudoLog.h"
#include "LudoGameModeBase.h"
#include "Game/GamerState.h"


void ALudoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALudoGameState, PlayerCountForGame);
	DOREPLIFETIME(ALudoGameState, CurrentPlayerIndex);
}

TObjectPtr<AGamerState> ALudoGameState::GetGamerStateForIndex(int8 PlayerIndex) const
{
	const TObjectPtr<APlayerState>* PlayerStatePtr = PlayerArray.FindByPredicate([PlayerIndex](const TObjectPtr<APlayerState>& PlayerState)
	{
		auto GamerState = Cast<AGamerState>(PlayerState);
		return (GamerState->GetPlayerIndex() == PlayerIndex);
	});

	if (!PlayerStatePtr) return nullptr;

	return CastChecked<AGamerState>(PlayerStatePtr->Get());
}

TObjectPtr<AGamerState> ALudoGameState::GetGamerStateInTurn() const
{
	return GetGamerStateForIndex(CurrentPlayerIndex);
}

bool ALudoGameState::IsPlayerTurn(TObjectPtr<APlayerController> Player)
{
	return true;
}

uint8 ALudoGameState::GetNumPlayersReady()
{
	uint8 ReadyCount = 0;
	for (auto p = PlayerArray.CreateConstIterator(); p; ++p)
	{
		if (TObjectPtr<AGamerState> GamerState = Cast<AGamerState>(*p))
		{
			if (GamerState->GetPlayState() == EPlayState::Ready)
			{
				ReadyCount++;
			}
		}
	}
	return ReadyCount;
}

uint8 ALudoGameState::GetNumPlayersReplicated()
{
	uint8 PlayerCount = 0;
	for (auto p = PlayerArray.CreateConstIterator(); p; ++p)
	{
		if (TObjectPtr<AGamerState> GamerState = Cast<AGamerState>(*p))
		{
			if (GamerState->GetPlayerIndex() > -1)
			{
				PlayerCount++;
			}
		}
	}
	return PlayerCount;
}

void ALudoGameState::AdvanceTurn()
{
	if (!HasAuthority()) return;

	UE_LOG(LogLudoGS, Verbose, TEXT("AdvanceTurn"));

	uint8 NumPlayers = PlayerArray.Num();

	if (NumPlayers == ++CurrentPlayerIndex)
	{
		CurrentPlayerIndex = 0;
	}

	OnTurnChangedNative.Broadcast(CurrentPlayerIndex);
}

void ALudoGameState::OnRep_CurrentPlayerIndex()
{
	OnTurnChangedNative.Broadcast(CurrentPlayerIndex);
}
