// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Yard.h"

#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>

#include "Game/LudoPlayerController.h"
#include "Actors/Gamer.h"
#include "Actors/PlayerSlot.h"
#include "Actors/Board.h"
#include "Actors/Piece.h"


AYard::AYard()
{
	PieceClass = APiece::StaticClass();
}

void AYard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AYard, Gamer);
}

void AYard::SetGamer(TObjectPtr<AGamer> NewGamer)
{
	if (!HasAuthority() || !NewGamer) return;

	Gamer = NewGamer;
	Gamer->SetYard(this);

	if (const TObjectPtr<APlayerSlot> PlayerSlot = NewGamer->GetPlayerSlot())
	{
		SetPlayerCore(PlayerSlot->PlayerCore);
	}
}

void AYard::OnRep_PlayerCore()
{
	Super::OnRep_PlayerCore();

	if (!PlayerSquares.IsEmpty())
	{
		for (APlayerSquare* Square : PlayerSquares)
		{
			Square->SetPlayerCore(PlayerCore);
		}
	}
}

void AYard::SpawnPieces()
{
	const FVector YardLocation = GetActorLocation();

	char PieceIndex = 0;
	for (char x = 0; x < 2; x++)
	{
		for (char y = 0; y < 2; y++)
		{
			constexpr float PieceGap = 150;
			FVector PieceTranslation;
			PieceTranslation.X = x*PieceGap - PieceGap*0.5;
			PieceTranslation.Y = y*PieceGap - PieceGap*0.5;

			FTransform PieceTransform;
			PieceTransform.SetLocation(YardLocation + PieceTranslation);

			TObjectPtr<AActor> ActorToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, PieceClass.LoadSynchronous(), PieceTransform);

			if (const TObjectPtr<APiece> Piece = Cast<APiece>(ActorToSpawn))
			{
				// Set PlayerCore, owner etc
				Piece->SetOwner(this);
				Piece->SetPlayerCore(this->PlayerCore);
				Piece->SetInYard(true);
				Piece->SetInitialLocation(PieceTransform.GetLocation());

				if (Gamer)
				{
					Piece->SetInstigator(Gamer.Get());
					Gamer->AddPiece(Piece);
				}

				// Add data to Board
				if (const TObjectPtr<ALudoPlayerController> PlayerController = GetWorld()->GetFirstPlayerController<ALudoPlayerController>())
				{
					const TObjectPtr<ABoard> TheBoard = PlayerController->GetBoard();
					TheBoard->AddPieceToBoardData(Piece, this);
				}
			}

			ActorToSpawn->FinishSpawning(PieceTransform);

			PieceIndex++;
		}
	}
}

TObjectPtr<APiece> AYard::GetPiece() const
{
	auto Pieces = GetGamer()->GetPieces();
	auto* FoundPiece = Pieces.FindByPredicate([&](const APiece* Piece)
	{
		return Piece->IsInYard();
	});

	return FoundPiece ? *FoundPiece : nullptr;
}

void AYard::DistributePieces(const ABoard* GameBoard) const
{
	// Do nothing
}
