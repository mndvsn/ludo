// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/PlayerSlot.h"

#include <Net/UnrealNetwork.h>

#include "Actors/Gamer.h"
#include "Game/GamerState.h"


APlayerSlot::APlayerSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
}

void APlayerSlot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerSlot, Index);
	DOREPLIFETIME(APlayerSlot, bClaimed);
	DOREPLIFETIME(APlayerSlot, Gamer);
	DOREPLIFETIME(APlayerSlot, PlayerCore);
	DOREPLIFETIME(APlayerSlot, PlayOrder);
}

bool APlayerSlot::TryClaim(AController* InController)
{
	CheckUnclaimed();
	if (IsClaimed() || !InController) return false;

	ClaimingController = InController;

	return bClaimed = true;
}

void APlayerSlot::CheckUnclaimed()
{
	//TODO: Make actual check if there is a controller using this slot
	bClaimed = ClaimingController ? true : false;
}

bool APlayerSlot::IsClaimed() const
{
	return bClaimed;
}

AGamerState* APlayerSlot::GetGamerState() const
{
	AGamerState* GamerState = nullptr;
	if (Gamer != nullptr)
	{
		GamerState = Gamer->GetPlayerState<AGamerState>();
	}
	return GamerState;
}
