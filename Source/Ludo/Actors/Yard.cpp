// Copyright Martin Furuberg. All Rights Reserved.


#include "Actors/Yard.h"
#include "Common/PlayerCore.h"


AYard::AYard()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AYard::BeginPlay()
{
	Super::BeginPlay();	
}
