// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
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

				if (Square->Tags.Contains(TEXT("goal")))
				{
					GoalSquare = Square;
				}
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

TObjectPtr<AYard> ABoard::GetYard(uint8 PlayerIndex) const
{
	if (Yards.IsEmpty()) return nullptr;

	return Yards[PlayerIndex];
}

TArray<TObjectPtr<APlayerSquare>> ABoard::GetPlayerSquares(uint8 PlayerIndex) const
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

uint8 ABoard::IndexOfSquare(ASquare* Square) const
{
	if (!Square) return INDEX_NONE;

	return Squares.Find(Square);
}

bool ABoard::PlayerHasPieceOnBoard(const int8 PlayerIndex) const
{
	const TObjectPtr<AGamer> Gamer = GetYard(PlayerIndex)->GetGamer();
	if (!Gamer) return false;

	return (Gamer->GetPiecesOnBoard(this).Num() > 0);
}

TObjectPtr<ASquare> ABoard::LocationOfPiece(TObjectPtr<APiece> Piece) const
{
	TObjectPtr<ASquare> Square = nullptr;

	for (const FSquareData& Data : BoardData)
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

TObjectPtr<APiece> ABoard::GetFirstPieceInYard(const TObjectPtr<AYard> InYard) const
{
	if (!InYard) return nullptr;
	auto Pieces = InYard->GetGamer()->GetPieces();
	auto* Piece = Pieces.FindByPredicate([&](const APiece* Piece)
	{
		return Piece->IsInYard();
	});

	return Piece ? *Piece : nullptr;
}

TArray<TObjectPtr<ASquare>> ABoard::GetReachableSquares(const int StartIndex, const int StepLimit, const uint8 ForPlayerIndex) const
{
	TArray<TObjectPtr<ASquare>> Reachable;

	if (!Squares.IsValidIndex(StartIndex) || Squares[StartIndex] == nullptr)
	{
		return Reachable;
	}

	// Get PlayerCore.Id of player
	ALudoGameState* GameState = GetWorld()->GetGameState<ALudoGameState>();
	const uint8 PlayerCoreId = GameState->GetPlayerSlot(ForPlayerIndex)->PlayerCore.Id;

	TQueue<TObjectPtr<ASquare>> Near;
	TMap<TObjectPtr<ASquare>, short> StepMap;
	TObjectPtr<ASquare> Current = Squares[StartIndex];

	Near.Enqueue(Current);
	StepMap.Add(Current, 0);

	while (Near.Dequeue(Current))
	{	
		if (StepMap[Current] >= StepLimit) break;

		// Process squares connected to current
		for (TObjectPtr<ASquare>& Next : Current->GetNext())
		{
			if (Reachable.Contains(Next)) continue;

			// Skip if the next square is an unreachable PlayerSquare
			if (TObjectPtr<APlayerSquare> PlayerSquare = Cast<APlayerSquare>(Next))
			{
				// Square is player color
				if (PlayerSquare->GetPlayerCore().Id == PlayerCoreId)
				{
					// Skip home; go toward goal
					if (PlayerSquare->IsHome())
					{
						continue;
					}
				}
				// Square is not players color, but ignore the not home squares
				else if (!PlayerSquare->IsHome())
				{
					continue;
				}
			}
			Near.Enqueue(Next);
			Reachable.Add(Next);
			StepMap.Add(Next, StepMap[Current]+1);
		}
	}
	return Reachable;
}

void ABoard::MovePiece_Implementation(APiece* Piece, ASquare* TargetSquare)
{
	// Fail if null pointers or Piece is at target location already
	if (!Piece || !TargetSquare || LocationOfPiece(Piece) == TargetSquare) return;

	RemovePieceFromBoardData(Piece);

	if (TargetSquare == GoalSquare)
	{
		TObjectPtr<ALudoGameModeBase> GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>();
		GameMode->PlayerPieceReachedGoal(Piece);
	}
	
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
