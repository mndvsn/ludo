// Copyright Martin Furuberg. All Rights Reserved.


#include "GamerState.h"
#include "Net/UnrealNetwork.h"

#include "Actors/Gamer.h"

AGamerState::AGamerState()
{
	// Use custom player names
	// expects GetPlayerNameCustom()
	//bUseCustomPlayerNames = true;
}

void AGamerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamerState, PlayerIndex);
	DOREPLIFETIME(AGamerState, PlayState);
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
	if (AController* OwnerController = GetOwner<AController>())
	{
		if (AGamer* Gamer = Cast<AGamer>(OwnerController->GetPawn()))
		{
			Gamer->UpdatePlayerLabel();
		}
	}
}

void AGamerState::OnRep_PlayerIndex()
{
	UE_LOG(LogTemp, Verbose, TEXT("Assigned PlayerIndex: %d"), PlayerIndex);
}

void AGamerState::SetPlayerIndex(int NewIndex)
{
	PlayerIndex = NewIndex;
	OnRep_PlayerIndex();
}

void AGamerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AGamerState* GamerState = CastChecked<AGamerState>(PlayerState))
	{
		GamerState->SetPlayerIndex(GetPlayerIndex());
	}
}

void AGamerState::OverrideWith(class APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

	if (AGamerState* GamerState = CastChecked<AGamerState>(PlayerState))
	{
		SetPlayerIndex(GamerState->PlayerIndex);
	}
}
