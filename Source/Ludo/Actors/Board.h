// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Actors/Square.h"
#include "Board.generated.h"


DECLARE_DELEGATE(FGE_OnBoardFoundYards);
DECLARE_DELEGATE(FGE_OnBoardMovePiecesComplete);

class AYard;
class APlayerSquare;

UCLASS(Abstract)
class LUDO_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	TArray<FSquareData> GetBoardData() const { return BoardData; }


	TObjectPtr<AYard> GetYard(uint8 PlayerIndex) const;

	TArray<TObjectPtr<APlayerSquare>> GetPlayerSquares(uint8 PlayerIndex) const;

	// Returns index of Square in array
	uint8 IndexOfSquare(ASquare* Square) const;

	bool PlayerHasPieceOnBoard(const int8 PlayerIndex) const;

	// Returns ASquare where Piece is currently located, nullptr if not found
	TObjectPtr<ASquare> LocationOfPiece(TObjectPtr<APiece> Piece) const;

	// Get Pieces at specific Square
	bool GetPiecesAtSquare(const TObjectPtr<const ASquare> TargetSquare, TArray<APiece*>& ResultArray) const;
	
	TArray<TObjectPtr<ASquare>> GetReachableSquares(const int StartIndex, const int StepLimit, const uint8 ForPlayerIndex) const;

	// Move Piece to square
	UFUNCTION(Server, Reliable)
	void MovePiece(APiece* Piece, ASquare* StartSquare, const TArray<ASquare*>& SquaresAhead);
	
	UFUNCTION(NetMulticast, Reliable)
	void PerformMove(APiece* Piece, const TArray<ASquare*>& Path, const TArray<ASquare*>& PostAffectedSquares);

	FGE_OnBoardMovePiecesComplete OnBoardMovePiecesComplete;
	bool bMovingPiece = false;

	// Knock Piece from square
	void KnockPiece(const TObjectPtr<APiece> Piece, const TObjectPtr<APiece> TargetPiece);

	void PerformKnock(APiece* Piece);
	
	FGE_OnBoardFoundYards OnFoundYards;
	bool bYardsFound = false;

	// BoardData manipulation
	bool AddPieceToBoardData(TObjectPtr<APiece> Piece, TObjectPtr<ASquare> TargetSquare);
	bool RemovePieceFromBoardData(TObjectPtr<APiece> Piece);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ASquare> GoalSquare;

	UPROPERTY()
	TArray<TObjectPtr<ASquare>> Squares;

	UPROPERTY()
	TArray<TObjectPtr<AYard>> Yards;

	UPROPERTY(Replicated)
	TArray<FSquareData> BoardData;
};
