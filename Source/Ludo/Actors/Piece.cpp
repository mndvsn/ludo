// Copyright Martin Furuberg. All Rights Reserved.


#include "Piece.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	// Color defaults to Red
	PieceColorIndex = EPieceColor::RED;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInterface* Material = StaticMesh->GetMaterial(0);
	ColorMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);

	ColorMaterialInstance->SetVectorParameterValue("Color", FLinearColor(APiece::PieceColors[(int)PieceColorIndex]));
	StaticMesh->SetMaterial(0, ColorMaterialInstance);
}

const FColor APiece::PieceColors[6] = {
	FColor::FromHex("cf2037"),
	FColor::FromHex("2f52a4"),
	FColor::FromHex("149b49"),
	FColor::FromHex("fcd20f"),
	FColor::FromHex("FFFFFF"),
	FColor::FromHex("231f20")
};
