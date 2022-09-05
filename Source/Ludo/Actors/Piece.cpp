// Copyright Martin Furuberg. All Rights Reserved.


#include "Piece.h"

#include <Net/UnrealNetwork.h>

#include "LudoLog.h"
#include "Actors/Gamer.h"


APiece::APiece()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	Super::SetReplicateMovement(true);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
}

void APiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APiece, PlayerCore);
	DOREPLIFETIME(APiece, bInYard);
	DOREPLIFETIME(APiece, bInGoal);
}

void APiece::OnRep_PlayerCore_Implementation()
{
	
}

void APiece::BeginPlay()
{
	Super::BeginPlay();
}

void APiece::SetPlayerCore(const FPlayerCore& InPlayerCore)
{
	PlayerCore = InPlayerCore;
}

AGamer* APiece::GetGamer() const
{
	return GetInstigator<AGamer>();
}
