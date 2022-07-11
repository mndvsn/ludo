// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"


UCLASS()
class LUDO_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Search(int StartIndex, int JumpLimit);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASquare> SquareClass;

	UPROPERTY()
	TArray<class ASquare*> Squares;

	UPROPERTY()
	int8 IndexInternal;

};
