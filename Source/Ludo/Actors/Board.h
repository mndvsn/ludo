// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"


DECLARE_DELEGATE(FGE_OnBoardFoundYards);

class ASquare;
class AYard;

UCLASS()
class LUDO_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();

	virtual void Tick(float DeltaTime) override;

	TObjectPtr<AYard> GetYard(uint8 PlayerIndex);

	void Search(int StartIndex, int JumpLimit);

	FGE_OnBoardFoundYards OnFoundYards;

	bool bYardsFound;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ASquare> SquareClass;

	UPROPERTY()
	TArray<TObjectPtr<ASquare>> Squares;

	UPROPERTY()
	TArray<TObjectPtr<AYard>> Yards;

};
