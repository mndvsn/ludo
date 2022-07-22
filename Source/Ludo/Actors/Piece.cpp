// Copyright Martin Furuberg. All Rights Reserved.


#include "Piece.h"
#include "LudoLog.h"
#include "Net/UnrealNetwork.h"
#include "Common/PlayerCore.h"


APiece::APiece()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
}

void APiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APiece, PlayerCore);
}

void APiece::OnRep_PlayerCore_Implementation()
{
	
}

void APiece::BeginPlay()
{
	Super::BeginPlay();
}

void APiece::SetPlayerCore(TObjectPtr<UPlayerCore> InPlayerCore)
{
	PlayerCore = InPlayerCore;
}
