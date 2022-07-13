// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Square.h"
#include "HomeSquare.generated.h"


class UPlayerCore;

/**
 * 
 */
UCLASS()
class LUDO_API AHomeSquare : public ASquare
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<UPlayerCore> PlayerCore;

};
