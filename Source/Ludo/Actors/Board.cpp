// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/Square.h"
#include "Actors/Yard.h"


ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;
	SquareClass = ASquare::StaticClass();

	bReplicates = true;

	// Billboard sprite size
	SpriteScale = 3.0f;
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
		for (TObjectPtr<AActor> Actor : FoundSquares)
		{
			if (TObjectPtr<ASquare> Square = Cast<ASquare>(Actor))
			{
				Squares.Add(Square);
			}
		}

		Algo::SortBy(Squares, &ASquare::Index);
	}

	// Find placed Yards
	TArray<TObjectPtr<AActor>> FoundYards;
	UGameplayStatics::GetAllActorsOfClass(this, AYard::StaticClass(), FoundYards);
	if (FoundYards.Num() > 0)
	{
		for (TObjectPtr<AActor> Actor : FoundYards)
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

void ABoard::Search(int StartIndex, int JumpLimit)
{
	for (TObjectPtr<ASquare> Sq : Squares)
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

		for (TObjectPtr<ASquare> Next : Current->GetNext())
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

