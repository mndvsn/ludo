// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"


class ASquare;

UCLASS()
class LUDO_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void SpawnSquares();

public:	
	void Search(int StartIndex, int JumpLimit);

private:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ASquare> SquareClass;

	UPROPERTY()
	TArray<TObjectPtr<ASquare>> Squares;

	UPROPERTY()
	int8 IndexInternal;

};
