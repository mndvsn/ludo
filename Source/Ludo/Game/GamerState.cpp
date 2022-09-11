// Copyright Martin Furuberg. All Rights Reserved.


#include "GamerState.h"

#include <Net/UnrealNetwork.h>

#include "LudoLog.h"
#include "LudoPlayerController.h"
#include "Actors/PlayerSlot.h"
#include "Actors/Gamer.h"

AGamerState::AGamerState()
{
	// Use custom player names
	// expects GetPlayerNameCustom()
	//bUseCustomPlayerNames = true;

	PlayState = EPlayState::Transitioning;
}

void AGamerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamerState, PlayerIndex);
	DOREPLIFETIME(AGamerState, PlayState);
	DOREPLIFETIME(AGamerState, PlayerSlot);
}

/**
 * Player name updated
 */
void AGamerState::UpdatePlayerName()
{
	OnRep_PlayerName();
}

void AGamerState::OnRep_PlayerName()
{
	if (const AController* OwnerController = GetOwner<AController>())
	{
		if (AGamer* Gamer = Cast<AGamer>(OwnerController->GetPawn()))
		{
			Gamer->UpdatePlayerLabel();
		}
	}
}

void AGamerState::OnRep_PlayerIndex() const
{
	UE_LOG(LogLudoGS, Verbose, TEXT("OnRep_PlayerIndex: %d (Client)"), PlayerIndex);
	// Check if this client is fully replicated and ready
	GetWorld()->GetFirstPlayerController<ALudoPlayerController>()->CheckPlayerStates();
}

void AGamerState::SetPlayerIndex(const uint8 NewIndex)
{
	UE_LOG(LogLudoGS, Verbose, TEXT("SetPlayerIndex: %d (Auth)"), NewIndex);
	PlayerIndex = NewIndex;
}

void AGamerState::SetPlayerSlot(const TObjectPtr<APlayerSlot> NewSlot)
{
	UE_LOG(LogLudoGS, Verbose, TEXT("SetPlayerSlot: %s > %d"), *GetPlayerName(), *NewSlot->GetName());
	PlayerSlot = NewSlot;
}

void AGamerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AGamerState* GamerState = CastChecked<AGamerState>(PlayerState))
	{
		GamerState->SetPlayerIndex(GetPlayerIndex());
		GamerState->SetPlayState(PlayState);
		GamerState->SetPlayerSlot(PlayerSlot);
	}
}

void AGamerState::OverrideWith(APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

	if (const AGamerState* GamerState = CastChecked<AGamerState>(PlayerState))
	{
		SetPlayerIndex(GamerState->PlayerIndex);
		SetPlayState(GamerState->PlayState);
		SetPlayerSlot(GamerState->PlayerSlot);
	}
}
