// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/PlayerSquare.h"

#include <Net/UnrealNetwork.h>


APlayerSquare::APlayerSquare()
{
	bReplicates = true;
}

void APlayerSquare::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerSquare, PlayerCore);
}

void APlayerSquare::SetPlayerCore(const FPlayerCore& NewPlayerCore)
{
	PlayerCore = NewPlayerCore;
	OnRep_PlayerCore();
}

void APlayerSquare::OnRep_PlayerCore()
{
	UpdatePlayerCore();
}
