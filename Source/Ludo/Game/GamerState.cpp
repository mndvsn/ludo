// Copyright Martin Furuberg. All Rights Reserved.


#include "GamerState.h"
#include "../Actors/Gamer.h"

AGamerState::AGamerState()
{
	// Use custom player names
	bUseCustomPlayerNames = true;
}

/**
 * Player name updated
 */
void AGamerState::OnRep_PlayerName()
{
	UpdatePlayerName();
}

void AGamerState::UpdatePlayerName()
{
	AController* OwnerController = GetOwner<AController>();
	if (OwnerController)
	{
		AGamer* Gamer = Cast<AGamer>(OwnerController->GetPawn());
		if (Gamer)
		{
			Gamer->UpdatePlayerLabel();
		}
	}
}
