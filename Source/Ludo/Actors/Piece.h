// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Piece.generated.h"

UENUM(BlueprintType)
enum class EPieceColor : uint8
{
	RED			UMETA(DisplayName = "Red"),
	BLUE		UMETA(DisplayName = "Blue"),
	GREEN		UMETA(DisplayName = "Green"),
	YELLOW		UMETA(DisplayName = "Yellow"),
	WHITE		UMETA(DisplayName = "White"),
	BLACK		UMETA(DisplayName = "Black")
};

UCLASS()
class LUDO_API APiece : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APiece();

	static const FColor PieceColors[6];

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	UMaterialInstanceDynamic* ColorMaterialInstance;

protected:
	UPROPERTY(EditAnywhere, Category = "Piece")
	EPieceColor PieceColorIndex;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
};