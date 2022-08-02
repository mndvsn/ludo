// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Yard.h"
#include "Kismet/GameplayStatics.h"

#include "LudoLog.h"
#include "Actors/Gamer.h"
#include "Actors/Piece.h"
#include "Common/PlayerCore.h"


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

// Called when the game starts or when spawned
void AYard::BeginPlay()
{
	Super::BeginPlay();	
}
