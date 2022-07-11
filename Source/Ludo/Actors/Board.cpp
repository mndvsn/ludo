// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Board.h"
#include "Math/UnrealMathUtility.h"

#include "Actors/Square.h"


ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;
	SquareClass = ASquare::StaticClass();
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	uint8 Items = 20;

	ASquare* Square = nullptr;
	ASquare* LastSquare = nullptr;

	double width = 850;
	double height = 850;

	double radius = 0.5 * FMath::Sqrt(FMath::Pow(width, 2) + FMath::Pow(height, 2));

	for (uint8 i = 0; i < Items; i++)
	{
		double deg = (PI*2 / Items) * i;
		double x, y;
		FMath::SinCos(&x, &y, deg);

		FVector SquarePos;
		SquarePos.X = FMath::Clamp(y * radius, -height / 2, height / 2);
		SquarePos.Y = FMath::Clamp(x * radius, -width / 2, width / 2);
		SquarePos.Z = 0;

		Square = GetWorld()->SpawnActor<ASquare>(SquareClass, SquarePos, FRotator::ZeroRotator);
		Square->Index = i;
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
		//Squares[0]->SetHighlight(true);
	}
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::Search(int StartIndex, int JumpLimit)
{
	for (ASquare* Sq : Squares)
	{
		Sq->SetHighlight(false);
	}
	///

	StartIndex += IndexInternal;

	if (!Squares.IsValidIndex(StartIndex) || Squares[StartIndex] == nullptr) return;

	ASquare* StartSquare = Squares[StartIndex];
	StartSquare->SetHighlight(true);

	TQueue<ASquare*> Near;
	Near.Enqueue(StartSquare);

	TSet<ASquare*> Reachable;
	Reachable.Add(StartSquare);

	short Jumps = 0;
	while (!Near.IsEmpty())
	{
		if (Jumps >= JumpLimit) break;

		ASquare* Current;
		Near.Dequeue(Current);

		for (ASquare* Next : Current->GetNext())
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

	IndexInternal++;
}

