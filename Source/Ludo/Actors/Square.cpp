// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Square.h"

#include <Components/SphereComponent.h>


ASquare::ASquare()
{
	PrimaryActorTick.bCanEverTick = true;
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
