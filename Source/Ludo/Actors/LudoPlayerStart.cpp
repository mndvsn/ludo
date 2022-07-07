// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/LudoPlayerStart.h"

void ALudoPlayerStart::SetPlayerSlot(uint8 SlotIndex)
{
	PlayerSlot = SlotIndex;
}

bool ALudoPlayerStart::TryClaim(AController* InController)
{
	CheckUnclaimed();
	if (IsClaimed() || !InController) return false;

	ClaimingController = InController;

	return Claimed = true;
}

void ALudoPlayerStart::CheckUnclaimed()
{
	//TODO: Make actual check if there is a controller using this slot
	Claimed = ClaimingController ? true : false;
}
