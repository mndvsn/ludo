// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "Actors/PlayerSquare.h"
#include "Yard.generated.h"


class APiece;
class AGamer;

UCLASS(Abstract)
class LUDO_API AYard : public APlayerSquare
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYard();

	void SpawnPieces();

	// Find first Piece located in Yard
	TObjectPtr<APiece> GetPiece() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Yard")
	TSoftClassPtr<APiece> PieceClass;

private:
	UPROPERTY(Replicated, BlueprintGetter=GetGamer)
	TObjectPtr<AGamer> Gamer;

public:	
	UFUNCTION(BlueprintPure)
	AGamer* GetGamer() const { return Gamer; };
	void SetGamer(TObjectPtr<AGamer> NewGamer);

	UFUNCTION(BlueprintPure)
	APlayerSquare* GetHomeSquare() const { return PlayerSquares[0]; };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<APlayerSquare*> PlayerSquares;

	virtual void OnRep_PlayerCore() override;
};
