// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Square.h"
#include "Board.generated.h"


DECLARE_DELEGATE(FGE_OnBoardFoundYards);

class AYard;
class APlayerSquare;

UCLASS()
class LUDO_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	TArray<FSquareData> GetBoardData() const { return BoardData; };

	TObjectPtr<AYard> GetYard(uint8 PlayerIndex);

	TArray<TObjectPtr<APlayerSquare>> GetPlayerSquares(uint8 PlayerIndex);

	bool PlayerHasPieceOnBoard(int8 PlayerIndex);

	// Returns ASquare where Piece is currently located, nullptr if not found
	TObjectPtr<ASquare> LocationOfPiece(TObjectPtr<APiece> Piece);

	// Find Piece located in Yard for player
	TObjectPtr<APiece> GetFirstPieceInYard(TObjectPtr<AYard> InYard);

	TArray<TObjectPtr<ASquare>> GetReachableSquares(int StartIndex, int StepLimit, uint8 ForPlayerIndex);

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
	TArray<TObjectPtr<ASquare>> Squares;

	UPROPERTY()
	TArray<TObjectPtr<AYard>> Yards;

	UPROPERTY(ReplicatedUsing=OnRep_BoardData)
	TArray<FSquareData> BoardData;

	UFUNCTION()
	void OnRep_BoardData();
};
