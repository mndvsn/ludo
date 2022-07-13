// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/Square.h"


ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;
	SquareClass = ASquare::StaticClass();

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

	//SpawnSquares();

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
}

void ABoard::SpawnSquares()
{
	uint8 Items = 20;

	ASquare* Square = nullptr;
	ASquare* LastSquare = nullptr;

	double width = 850;
	double height = 850;

	double radius = 0.5 * FMath::Sqrt(FMath::Pow(width, 2) + FMath::Pow(height, 2));

	for (uint8 i = 0; i < Items; i++)
	{
		double deg = (PI * 2 / Items) * i;
		double x, y;
		FMath::SinCos(&x, &y, deg);

		FVector SquarePos;
		SquarePos.X = FMath::Clamp(y * radius, -height / 2, height / 2);
		SquarePos.Y = FMath::Clamp(x * radius, -width / 2, width / 2);
		SquarePos.Z = 0;

		FTransform SquareTransform;
		SquareTransform.SetLocation(SquarePos);

		TObjectPtr<AActor> ActorSpawned = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, SquareClass.LoadSynchronous(), SquareTransform);

		Square = Cast<ASquare>(ActorSpawned);
		if (Square)
		{
			Square->Index = i;
		}
		ActorSpawned->FinishSpawning(SquareTransform);

		Square->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		Squares.Add(Square);

		if (LastSquare != nullptr) {
			LastSquare->AddNext(Square);
		}
		LastSquare = Square;
	}

	if (!Squares.IsEmpty())
	{
		LastSquare->AddNext(Squares[0]);
	}
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

