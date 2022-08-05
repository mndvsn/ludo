// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Square.generated.h"


class APiece;

/**
 *
 */
USTRUCT(BlueprintType)
struct FSquareData
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 Index;

	UPROPERTY()
	TObjectPtr<ASquare> Square;

	UPROPERTY()
	TArray<TObjectPtr<APiece>> Pieces;
};

/**
 *
 */
UCLASS()
class LUDO_API ASquare : public AActor
{
	GENERATED_BODY()
	
public:	
	ASquare();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	
	void AddNext(TObjectPtr<ASquare> NewSquare);
	TArray<TObjectPtr<ASquare>> GetNext() const { return Next; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ASquare>> Next;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetHighlight(bool Highlighted);

	FString GetDebugText() const;

	UPROPERTY(EditAnywhere)
	int32 Index = -1;
};
