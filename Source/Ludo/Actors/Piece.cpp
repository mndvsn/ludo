// Copyright Martin Furuberg. All Rights Reserved.


#include "Piece.h"
#include "LudoLog.h"
#include "Net/UnrealNetwork.h"


APiece::APiece()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

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
}

void APiece::OnRep_PlayerCore_Implementation()
{
	
}

void APiece::BeginPlay()
{
	Super::BeginPlay();
}

void APiece::SetPlayerCore(FPlayerCore InPlayerCore)
{
	PlayerCore = InPlayerCore;
}
