// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Yard.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "LudoLog.h"
#include "Actors/Gamer.h"
#include "Actors/PlayerSlot.h"
#include "Actors/PlayerSquare.h"
#include "Actors/Piece.h"


AYard::AYard()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	PieceClass = APiece::StaticClass();
}

void AYard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AYard, PlayerCore);
}

void AYard::SetGamer(TObjectPtr<AGamer> NewGamer)
{
	if (!NewGamer) return;

	Gamer = NewGamer;
	Gamer->SetYard(this);

	if (TObjectPtr<APlayerSlot> PlayerSlot = NewGamer->GetPlayerSlot())
	{
		PlayerCore = PlayerSlot->PlayerCore;

		OnRep_PlayerCore();
	}
}

void AYard::SpawnPieces()
{
	constexpr float PieceGap = 150;
	const FVector YardLocation = GetActorLocation();

	char PieceIndex = 0;
	for (char x = 0; x < 2; x++)
	{
		for (char y = 0; y < 2; y++)
		{
			FVector PieceTranslation;
			PieceTranslation.X = x*PieceGap - PieceGap*0.5;
			PieceTranslation.Y = y*PieceGap - PieceGap*0.5;

			FTransform PieceTransform;
			PieceTransform.SetLocation(YardLocation + PieceTranslation);

			TObjectPtr<AActor> ActorToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, PieceClass.LoadSynchronous(), PieceTransform);

			if (TObjectPtr<APiece> Piece = Cast<APiece>(ActorToSpawn))
			{
				// Set playercore, owner etc
				Piece->SetOwner(this);
				Piece->SetPlayerCore(this->PlayerCore);
				Piece->SetInYard(true);

				if (Gamer)
				{
					Piece->SetInstigator(Gamer.Get());
					Gamer->AddPiece(Piece);
				}
			}

			ActorToSpawn->FinishSpawning(PieceTransform);

			PieceIndex++;
		}
	}
}

void AYard::BeginPlay()
{
	Super::BeginPlay();	
}

void AYard::OnRep_PlayerCore_Implementation()
{
	if (!PlayerSquares.IsEmpty())
	{
		for (APlayerSquare* Square : PlayerSquares)
		{
			Square->SetPlayerCore(PlayerCore);
		}
	}
}
