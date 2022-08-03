// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "LudoLog.h"
#include "Game/LudoGameState.h"
#include "Actors/Yard.h"
#include "Actors/PlayerSquare.h"
#include "Actors/PlayerSlot.h"

#include "Common/PlayerCore.h"


ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;
	SquareClass = ASquare::StaticClass();

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
		for (TObjectPtr<AActor> Actor : FoundSquares)
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

	for (auto Square : Squares)
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

void ABoard::OnRep_BoardData()
{

}
