// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoGameState.h"

#include <Net/UnrealNetwork.h>

#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
#include "Game/GamerState.h"
#include "Game/LudoPlayerController.h"
#include "Actors/PlayerSlot.h"
#include "Actors/Piece.h"


void ALudoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALudoGameState, GameSettings);
	DOREPLIFETIME(ALudoGameState, RandomStream);
	DOREPLIFETIME(ALudoGameState, PlayerSlots);
	DOREPLIFETIME(ALudoGameState, PlayerPiecesInGoal);
	DOREPLIFETIME(ALudoGameState, CurrentPlayerIndex);
	DOREPLIFETIME(ALudoGameState, WinnerPlayerIndex);
	DOREPLIFETIME(ALudoGameState, DieThrowList);
}

TObjectPtr<AGamerState> ALudoGameState::GetGamerStateForIndex(int8 PlayerIndex) const
{
	const TObjectPtr<APlayerState>* PlayerStatePtr = PlayerArray.FindByPredicate([PlayerIndex](const TObjectPtr<APlayerState>& PlayerState)
	{
		const auto GamerState = Cast<AGamerState>(PlayerState);
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
	const auto GamerState = Player->GetPlayerState<AGamerState>();
	return (GamerState->GetPlayerIndex() == CurrentPlayerIndex);
}

uint8 ALudoGameState::GetNumPlayersReady() const
{
	uint8 ReadyCount = 0;
	for (auto p = PlayerArray.CreateConstIterator(); p; ++p)
	{
		if (const TObjectPtr<AGamerState> GamerState = Cast<AGamerState>(*p))
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
		if (const TObjectPtr<AGamerState> GamerState = Cast<AGamerState>(*p))
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

	// Reset index if number of players is equal to current player +1
	if (PlayerArray.Num() == ++CurrentPlayerIndex)
	{
		CurrentPlayerIndex = 0;
	}
	OnTurnChangedNative.Broadcast(CurrentPlayerIndex);
}

void ALudoGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		// Check if this client is fully replicated and ready
		GetWorld()->GetFirstPlayerController<ALudoPlayerController>()->CheckPlayerStates();
	}
}

void ALudoGameState::OnRep_CurrentPlayerIndex() const
{
	OnTurnChangedNative.Broadcast(CurrentPlayerIndex);
}


void ALudoGameState::SetPlayerSlots(TArray<APlayerSlot*> InPlayerSlots)
{
	if (!HasAuthority()) return;

	if (InPlayerSlots.Num() > 0)
	{
		PlayerSlots = InPlayerSlots;

		short SlotNum = PlayerSlots.Num();
		while (SlotNum--)
		{
			PlayerPiecesInGoal.Add(0);
		}
	}
}

APlayerSlot* ALudoGameState::GetPlayerSlot(uint8 PlayerIndex) const
{
	if (!PlayerSlots.IsValidIndex(PlayerIndex)) return nullptr;

	return PlayerSlots[PlayerIndex];
}

APlayerSlot* ALudoGameState::GetPlayerSlot(const FPlayerCore& PlayerCore) const
{
	const auto PlayerSlot = PlayerSlots.FindByPredicate([PlayerCore](const APlayerSlot* Slot)
	{
		return Slot->GetPlayerCore() == PlayerCore;
	});

	return PlayerSlot ? *PlayerSlot : nullptr;
}

bool ALudoGameState::HasGameEnded()
{
	bool bEnded = false;

	for (uint8 i=0; i < PlayerPiecesInGoal.Num(); i++)
	{
		const uint8 PlayerTotal = PlayerPiecesInGoal[i];
		
		if (PlayerTotal == 4)
		{
			bEnded = true;
			WinnerPlayerIndex = i;
			ForceNetUpdate();
		}
	}

	return bEnded;
}

void ALudoGameState::AddDieThrow(FDieThrow Throw)
{
	if (!HasAuthority()) return;

	DieThrowList.Add(Throw);

	OnRep_DieThrowList();
}

void ALudoGameState::AddPlayerPieceInGoal(uint8 PlayerIndex)
{
	if (!HasAuthority()) return;

	if (PlayerPiecesInGoal.IsValidIndex(PlayerIndex) && PlayerPiecesInGoal[PlayerIndex] < 4)
	{
		PlayerPiecesInGoal[PlayerIndex]++;

		OnPlayerReachedGoalNative.Broadcast(PlayerIndex, PlayerPiecesInGoal[PlayerIndex]);
	}
}

void ALudoGameState::OnRep_DieThrowList()
{
	const FDieThrow Throw = GetLastThrow();
	OnDieThrowNative.Broadcast(Throw);
}

void ALudoGameState::SetGameSettings(const FGameSettings& NewSettings)
{
	UE_LOG(LogLudoGS, Verbose, TEXT("Setting game settings to game state"));
	GameSettings = NewSettings;
	
	SetRandomSeed(GameSettings.Seed);
}

void ALudoGameState::SetRandomSeed(const int32 InSeed)
{
	if (RandomStream.GetCurrentSeed() != InSeed)
	{
		RandomStream.Initialize(InSeed);
	}
}
