// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"

#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>

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

				if (HasAuthority())
				{
					BoardData.AddUnique(SquareData);
				}

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
		(void)OnFoundYards.ExecuteIfBound();
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
	const TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>();
	const TObjectPtr<AGamerState> GamerState = GameState->GetGamerStateForIndex(PlayerIndex);
	if (!GamerState || !GamerState->GetPlayerSlot())
	{
		return SquareArray;
	}

	const FPlayerCore PlayerCore = GamerState->GetPlayerSlot()->GetPlayerCore();

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

//TODO: Currently returns index in array, not Square->Index
uint8 ABoard::IndexOfSquare(ASquare* Square) const
{
	if (!Square) return INDEX_NONE;

	return Squares.Find(Square);
}

bool ABoard::PlayerHasPieceOnBoard(const int8 PlayerIndex) const
{
	const TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>();
	const TObjectPtr<AGamer> Gamer = GameState->GetPlayerSlot(PlayerIndex)->GetGamer();
	if (!Gamer) return false;

	return (Gamer->GetPiecesOnBoard(this).Num() > 0);
}

TObjectPtr<ASquare> ABoard::LocationOfPiece(TObjectPtr<APiece> Piece) const
{
	TObjectPtr<ASquare> Square = nullptr;

	for (const FSquareData& Data : BoardData)
	{
		if (Data.Pieces.IsEmpty()) continue;

		if (Data.Pieces.Contains(Piece) && Data.Square.IsValid())
		{
			Square = Data.Square.Get();
			break;
		}
	}

	return Square;
}

bool ABoard::GetPiecesAtSquare(const TObjectPtr<const ASquare> TargetSquare, TArray<APiece*>& ResultArray) const
{
	bool bSuccess = false;
	if (!TargetSquare) return bSuccess;

	auto MatchingSquare = [TargetSquare](const FSquareData& Data)
	{
		return Data.Square == TargetSquare;
	};
	
	if (const auto FoundData = BoardData.FindByPredicate(MatchingSquare); !FoundData->Pieces.IsEmpty())
	{
		ResultArray.Append(FoundData->Pieces);
		bSuccess = true;
	}
	
	return bSuccess;
}

TArray<TObjectPtr<ASquare>> ABoard::GetReachableSquares(const int StartIndex, const int StepLimit, const uint8 ForPlayerIndex) const
{
	TArray<TObjectPtr<ASquare>> Reachable;

	if (!Squares.IsValidIndex(StartIndex) || Squares[StartIndex] == nullptr)
	{
		return Reachable;
	}

	// Get PlayerCore.Id of player
	const ALudoGameState* GameState = GetWorld()->GetGameState<ALudoGameState>();
	const uint8 PlayerCoreId = GameState->GetPlayerSlot(ForPlayerIndex)->GetPlayerCore().Id;

	TQueue<TObjectPtr<ASquare>> Near;
	TMap<TObjectPtr<ASquare>, int> StepMap;
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
			if (const TObjectPtr<APlayerSquare> PlayerSquare = Cast<APlayerSquare>(Next))
			{
				// Square is player color
				if (PlayerSquare->GetPlayerCore().Id == PlayerCoreId)
				{
					// Skip home; go toward goal. But only if current square is not the Yard. 
					if (PlayerSquare->IsHome() && !Cast<AYard>(Current))
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

void ABoard::MovePiece_Implementation(APiece* Piece, ASquare* StartSquare, const TArray<ASquare*>& SquaresAhead)
{
	// Fail if null pointers or Piece is at target location already
	if (!Piece || !StartSquare || SquaresAhead.IsEmpty()) return;
	const TObjectPtr<ASquare> TargetSquare = SquaresAhead.Last();
	if (StartSquare == TargetSquare) return;

	const TObjectPtr<ALudoGameModeBase> GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>();
	
	// Get game rules
	const TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>();
	const bool bKnock = GameState->GetSettings().bKnockPieces; // knock-out opponent pieces
	const bool bKnockMultiple = GameState->GetSettings().bKnockMultiple; // multiple

	bMovingPiece = true;
	
	RemovePieceFromBoardData(Piece);

	if (TargetSquare == GoalSquare)
	{
		GameMode->PlayerPieceReachedGoal(Piece);
	}

	// Check if target is occupied with other Pieces
	TArray<APiece*> PiecesAtTarget;
	if (TargetSquare != GoalSquare && GetPiecesAtSquare(TargetSquare, PiecesAtTarget))
	{
		FString NamesOfPieces;
		TArray<TObjectPtr<APiece>> OpponentPieces;
		for (const auto& OccupyingPiece : PiecesAtTarget)
		{
			NamesOfPieces += OccupyingPiece->GetName();
			NamesOfPieces += TEXT(", ");
			if (Piece->GetPlayerCore() != OccupyingPiece->GetPlayerCore())
			{
				OpponentPieces.Add(OccupyingPiece);
				if (!bKnockMultiple) break;
			}
		}
		UE_LOG(LogLudo, Verbose, TEXT("Pieces already on %s: %s"), *TargetSquare->GetName(), *NamesOfPieces);

		// Knock-out is enabled
		if (bKnock && !OpponentPieces.IsEmpty())
		{
			for (const auto& OpponentPiece : OpponentPieces)
			{
				KnockPiece(Piece, OpponentPiece);
			}
		}
	}
	
	if (AddPieceToBoardData(Piece, TargetSquare))
	{
		if (Piece->IsInYard())
		{
			Piece->SetInYard(false);
		}

		// Tidy up squares
		const TArray<ASquare*> SquaresToTidy = { StartSquare, TargetSquare };

		// Make client move the Piece
		PerformMove(Piece, SquaresAhead, SquaresToTidy);
	}
}

void ABoard::PerformMove_Implementation(APiece* Piece, const TArray<ASquare*>& Path,
	const TArray<ASquare*>& PostAffectedSquares)
{
	if (!Piece) return;
	
	// Make movement vectors
	TArray<FVector> Moves;
	for (auto& Square : Path)
	{
		Moves.Add(Square->GetActorLocation());
	}
	
	// Post animation
	Piece->GetAnimatePathFinishedDelegate().BindWeakLambda(this, [this, Piece, PostAffectedSquares]
	{
		// Check if this Piece hit opponent
		if (Piece->GetKnockedPieces().Num() > 0)
		{
			for (APiece* KnockedPiece : Piece->GetKnockedPieces())
			{
				PerformKnock(KnockedPiece);
			}
			Piece->ResetKnockedPieces();
		}

		// Tidy up Pieces at start/stop square
		for (auto& Square : PostAffectedSquares)
		{
			Square->DistributePieces(this);
		}
		bMovingPiece = false;
		(void) OnBoardMovePiecesComplete.ExecuteIfBound();
		Piece->GetAnimatePathFinishedDelegate().Unbind();
	});

	// Animate movement
	Piece->AnimatePath(Moves, true);
}

void ABoard::KnockPiece(const TObjectPtr<APiece> Piece, const TObjectPtr<APiece> TargetPiece)
{
	if (!HasAuthority()) return;
	
	// Reset Piece back to initial location in Yard
	if (RemovePieceFromBoardData(TargetPiece))
	{
		UE_LOG(LogLudo, Verbose, TEXT("KNOCK! %s (%s) moved back to yard"), *TargetPiece->GetName(), *TargetPiece->GetPlayerCore().DisplayName);
		Piece->AddKnockedPiece(TargetPiece);
		TargetPiece->SetInYard(true);
		AddPieceToBoardData(TargetPiece, TargetPiece->GetOwner<AYard>());
	}
}

void ABoard::PerformKnock(APiece* Piece)
{
	// Make movement vector
	const TArray MoveBackToInitial = { Piece->GetInitialLocation() };
	
	// Animate movement
	Piece->AnimatePath(MoveBackToInitial);
}

bool ABoard::AddPieceToBoardData(TObjectPtr<APiece> Piece, TObjectPtr<ASquare> TargetSquare)
{
	bool bSuccess = false;
	if (!Piece || !TargetSquare) return bSuccess;

	UE_LOG(LogLudo, Verbose, TEXT("Moving %s (%s) to %s"), *Piece->GetName(), *Piece->GetPlayerCore().DisplayName, *TargetSquare->GetName());

	const int32 ArrayIndex = BoardData.IndexOfByPredicate([TargetSquare](const FSquareData& Data)
	{
		return (Data.Square == TargetSquare);
	});

	if (ArrayIndex > INDEX_NONE)
	{
		BoardData[ArrayIndex].Pieces.AddUnique(Piece);
		bSuccess = true;
	}

	return bSuccess;
}

bool ABoard::RemovePieceFromBoardData(TObjectPtr<APiece> Piece)
{
	bool bSuccess = false;
	if (!Piece) return bSuccess;

	const int32 ArrayIndex = BoardData.IndexOfByPredicate([Piece](const FSquareData& Data)
	{
		return Data.Pieces.Contains(Piece);
	});

	if (ArrayIndex > INDEX_NONE)
	{
		BoardData[ArrayIndex].Pieces.RemoveSingle(Piece);
		bSuccess = true;
	}

	return bSuccess;
}

