// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoGameState.h"
#include "Net/UnrealNetwork.h"

#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
#include "Game/GamerState.h"
#include "Actors/Piece.h"


void ALudoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALudoGameState, PlayerCountForGame);
	DOREPLIFETIME(ALudoGameState, PlayerSlots);
	DOREPLIFETIME(ALudoGameState, CurrentPlayerIndex);
	DOREPLIFETIME(ALudoGameState, DieThrowList);
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

bool ALudoGameState::IsPlayerTurn(TObjectPtr<APlayerController> Player) const
{
	auto GamerState = Player->GetPlayerState<AGamerState>();
	return (GamerState->GetPlayerIndex() == CurrentPlayerIndex);
}

bool ALudoGameState::PlayerHasPieceOnBoard(int8 PlayerIndex) const
{
	return false;
}

uint8 ALudoGameState::GetNumPlayersReady() const
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

uint8 ALudoGameState::GetNumPlayersReplicated() const
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

APlayerSlot* ALudoGameState::GetPlayerSlot(uint8 PlayerIndex)
{
	if (!PlayerSlots.IsValidIndex(PlayerIndex)) return nullptr;

	return PlayerSlots[PlayerIndex];
}

void ALudoGameState::AddDieThrow(FDieThrow Throw)
{
	if (!HasAuthority()) return;

	DieThrowList.Add(Throw);

	OnRep_DieThrowList();
}

void ALudoGameState::OnRep_DieThrowList()
{
	FDieThrow Throw = GetLastThrow();
	OnDieThrowNative.Broadcast(Throw);
}
