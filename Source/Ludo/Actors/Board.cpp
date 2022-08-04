// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "LudoLog.h"
#include "Game/LudoGameState.h"
#include "Actors/Gamer.h"
#include "Actors/Piece.h"
#include "Actors/Yard.h"
#include "Actors/PlayerSquare.h"
#include "Actors/PlayerSlot.h"

#include "Common/PlayerCore.h"


ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	// Billboard sprite size
	SpriteScale = 3.0f;
}

void ABoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoard, BoardData);
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	// Find placed Squares
	TArray<TObjectPtr<AActor>> FoundSquares;
	UGameplayStatics::GetAllActorsOfClass(this, ASquare::StaticClass(), FoundSquares);
	if (FoundSquares.Num() > 0)
	{
		for (TObjectPtr<AActor>& Actor : FoundSquares)
		{
			if (TObjectPtr<ASquare> Square = Cast<ASquare>(Actor))
			{
				Squares.Add(Square);

				FSquareData SquareData;
				SquareData.Index = Square->Index;
				SquareData.Square = Square;

				BoardData.Add(SquareData);
			}
		}

		Algo::SortBy(Squares, &ASquare::Index);
		Algo::SortBy(BoardData, &FSquareData::Index);
	}

	// Find placed Yards
	TArray<TObjectPtr<AActor>> FoundYards;
	UGameplayStatics::GetAllActorsOfClass(this, AYard::StaticClass(), FoundYards);
	if (FoundYards.Num() > 0)
	{
		for (TObjectPtr<AActor>& Actor : FoundYards)
		{
			if (TObjectPtr<AYard> Yard = Cast<AYard>(Actor))
			{
				Yards.Add(Yard);
			}
		}

		Algo::SortBy(Yards, &AYard::Index);

		bYardsFound = true;
		OnFoundYards.ExecuteIfBound();
	}
}

TObjectPtr<AYard> ABoard::GetYard(uint8 PlayerIndex)
{
	if (Yards.IsEmpty()) return nullptr;

	return Yards[PlayerIndex];
}

TArray<TObjectPtr<APlayerSquare>> ABoard::GetPlayerSquares(uint8 PlayerIndex)
{
	TArray<TObjectPtr<APlayerSquare>> SquareArray;

	// Find PlayerCore used by player
	TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>();
	TObjectPtr<AGamerState> GamerState = GameState->GetGamerStateForIndex(PlayerIndex);
	if (!GamerState || !GamerState->GetPlayerSlot())
	{
		return SquareArray;
	}

	FPlayerCore PlayerCore = GamerState->GetPlayerSlot()->PlayerCore;

	for (auto& Square : Squares)
	{
		if (auto PlayerSquare = Cast<APlayerSquare>(Square))
		{
			if (PlayerSquare->GetPlayerCore() == PlayerCore)
			{
				SquareArray.Add(PlayerSquare);
			}
		}
	}
	return SquareArray;
}

bool ABoard::PlayerHasPieceOnBoard(int8 PlayerIndex)
{
	bool bHasPiece = false;
	TObjectPtr<AYard> PlayerYard = GetYard(PlayerIndex);

	for (FSquareData& Square : BoardData)
	{
		if (Square.Pieces.IsEmpty()) continue;

		bHasPiece = Square.Pieces.ContainsByPredicate([PlayerYard](const TObjectPtr<APiece> Piece)
		{
			return (Piece->GetOwner() == PlayerYard);
		});

		if (bHasPiece) break;
	}

	return bHasPiece;
}

TObjectPtr<ASquare> ABoard::LocationOfPiece(TObjectPtr<APiece> Piece)
{
	TObjectPtr<ASquare> Square = nullptr;

	for (FSquareData& Data : BoardData)
	{
		if (Data.Pieces.IsEmpty()) continue;

		if (Data.Pieces.Contains(Piece))
		{
			Square = Data.Square;
			break;
		}
	}

	return Square;
}

TObjectPtr<APiece> ABoard::GetFirstPieceInYard(TObjectPtr<AYard> InYard)
{
	if (!InYard) return nullptr;
	auto Pieces = InYard->GetGamer()->GetPieces();
	auto* Piece = Pieces.FindByPredicate([&](const APiece* Piece)
	{
		return Piece->IsInYard();
	});

	return Piece ? *Piece : nullptr;
}

void ABoard::Search(int StartIndex, int JumpLimit)
{
	for (TObjectPtr<ASquare>& Sq : Squares)
	{
		Sq->SetHighlight(false);
	}
	///

	if (!Squares.IsValidIndex(StartIndex) || Squares[StartIndex] == nullptr) return;

	ASquare* StartSquare = Squares[StartIndex];
	StartSquare->SetHighlight(true);

	TQueue<TObjectPtr<ASquare>> Near;
	Near.Enqueue(StartSquare);

	TSet<TObjectPtr<ASquare>> Reachable;
	Reachable.Add(StartSquare);

	short Jumps = 0;
	while (!Near.IsEmpty())
	{
		if (Jumps >= JumpLimit) break;

		TObjectPtr<ASquare> Current;
		Near.Dequeue(Current);

		for (TObjectPtr<ASquare>& Next : Current->GetNext())
		{
			if (!Reachable.Contains(Next))
			{
				Near.Enqueue(Next);
				Reachable.Add(Next);

				Next->SetHighlight(true);
				Jumps++;
			}
		}
	}
}

void ABoard::MovePiece_Implementation(APiece* Piece, ASquare* TargetSquare)
{
	// Fail if null pointers or Piece is at target location already
	if (!Piece || !TargetSquare || LocationOfPiece(Piece) == TargetSquare) return;

	RemovePieceFromBoardData(Piece);

	if (AddPieceToBoardData(Piece, TargetSquare))
	{
		if (Piece->IsInYard())
		{
			Piece->SetInYard(false);
		}
		Piece->SetActorLocation(TargetSquare->GetActorLocation());
	}
}

bool ABoard::AddPieceToBoardData(TObjectPtr<APiece> Piece, TObjectPtr<ASquare> TargetSquare)
{
	bool bSuccess = false;
	if (!Piece || !TargetSquare) return bSuccess;

	UE_LOG(LogLudo, Verbose, TEXT("Moving %s (%s) to %s"), *Piece->GetName(), *Piece->GetPlayerCore().DisplayName, *TargetSquare->GetName());

	int32 ArrayIndex = BoardData.IndexOfByPredicate([TargetSquare](const FSquareData& Data)
	{
		return (Data.Square == TargetSquare);
	});

	if (ArrayIndex > INDEX_NONE)
	{
		BoardData[ArrayIndex].Pieces.Add(Piece);
		bSuccess = true;
	}

	return bSuccess;
}

bool ABoard::RemovePieceFromBoardData(TObjectPtr<APiece> Piece)
{
	bool bSuccess = false;
	if (!Piece) return bSuccess;

	int32 ArrayIndex = BoardData.IndexOfByPredicate([Piece](const FSquareData& Data)
	{
		return (Data.Pieces.Contains(Piece));
	});

	if (ArrayIndex > INDEX_NONE)
	{
		BoardData[ArrayIndex].Pieces.RemoveSingle(Piece);
		bSuccess = true;
	}

	return bSuccess;
}

void ABoard::OnRep_BoardData()
{

}
