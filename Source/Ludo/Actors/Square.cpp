// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Square.h"

#include "Yard.h"
#include "Actors/Board.h" 
#include "Actors/Piece.h"


ASquare::ASquare()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickInterval(10.0);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
}

void ASquare::AddNext(TObjectPtr<ASquare> NewSquare)
{
	Next.Add(NewSquare);
}

void ASquare::DistributePieces(const ABoard* GameBoard) const
{
	if (!GameBoard) return;
	
	TArray<TObjectPtr<APiece>> Pieces;
	if (GameBoard->GetPiecesAtSquare(this, Pieces))
	{
		const FVector Origin = GetActorLocation();
		double Radius = 0;
		
		const char Items = Pieces.Num();
		if (Items > 1)
		{
			// 70% of Square radius
			Radius = 0.7 * StaticMesh->GetStaticMesh()->GetBoundingBox().GetExtent().Size();
		}

		const float Slice = PI*2 / Items;
		for (short i = 0; i < Pieces.Num(); i++)
		{
			const TObjectPtr<APiece>& Piece = Pieces[i];
			FVector NewLocation = Origin;
			NewLocation.X += Radius * FMath::Cos(Slice * i);
			NewLocation.Y += Radius * FMath::Sin(Slice * i);

			Piece->SetActorLocation(NewLocation);
		}
	}
}

void ASquare::BeginPlay()
{
	Super::BeginPlay();
}

void ASquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugString(GetWorld(), FVector(0, 0, 50), GetDebugText(), this, FColor::Red);
}

FString ASquare::GetDebugText() const
{
	FString Debug = FString::Printf(TEXT("%d"), Index);
	if (!GetNext().IsEmpty())
	{
		Debug += FString::Printf(TEXT(" (%d)"), GetNext().Num());
	}
	return Debug;
}
