// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Actors/Square.h"
#include "Board.generated.h"


DECLARE_DELEGATE(FGE_OnBoardFoundYards);

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

	// Find Piece located in Yard for player
	TObjectPtr<APiece> GetFirstPieceInYard(const TObjectPtr<AYard> InYard) const;

	TArray<TObjectPtr<ASquare>> GetReachableSquares(const int StartIndex, const int StepLimit, const uint8 ForPlayerIndex) const;

	UFUNCTION(Server, Reliable)
	void MovePiece(APiece* Piece, ASquare* TargetSquare);

	FGE_OnBoardFoundYards OnFoundYards;
	bool bYardsFound;

protected:
	virtual void BeginPlay() override;

	// BoardData manipulation
	bool AddPieceToBoardData(TObjectPtr<APiece> Piece, TObjectPtr<ASquare> TargetSquare);
	bool RemovePieceFromBoardData(TObjectPtr<APiece> Piece);

private:
	UPROPERTY()
	TObjectPtr<ASquare> GoalSquare;

	UPROPERTY()
	TArray<TObjectPtr<ASquare>> Squares;

	UPROPERTY()
	TArray<TObjectPtr<AYard>> Yards;

	UPROPERTY(ReplicatedUsing=OnRep_BoardData)
	TArray<FSquareData> BoardData;

	UFUNCTION()
	void OnRep_BoardData();
};
