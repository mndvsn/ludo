// Copyright Martin Furuberg. All Rights Reserved.


#include "Piece.h"

#include <Net/UnrealNetwork.h>

#include "LudoLog.h"
#include "Actors/Gamer.h"


APiece::APiece()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = true;
	Super::SetReplicateMovement(false);

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
	
	SetInitialLocation(GetActorLocation());
}

void APiece::SetPlayerCore(const FPlayerCore& InPlayerCore)
{
	PlayerCore = InPlayerCore;
}

AGamer* APiece::GetGamer() const
{
	return GetInstigator<AGamer>();
}

void APiece::AnimatePath_Implementation(const TArray<FVector>& Path, const bool bHandleFinished)
{
	MovesArray = Path;
	if (bHandleFinished)
	{
		OnAnimatePathFinished.AddUniqueDynamic(this, &APiece::HandleAnimatePathFinished);
	}
}

void APiece::HandleAnimatePathFinished()
{
	(void)OnAnimatePathFinishedNative.ExecuteIfBound();
	OnAnimatePathFinished.RemoveAll(this);
}
