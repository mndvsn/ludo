// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Square.h"
#include "PlayerSquare.generated.h"


class UPlayerCore;

/**
 * 
 */
UCLASS()
class LUDO_API APlayerSquare : public ASquare
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter=GetPlayerCore, Category="Square")
	TObjectPtr<UPlayerCore> PlayerCore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Square")
	bool bIsHome;

public:
	UFUNCTION(BlueprintPure)
	bool IsHome() const { return bIsHome; };

	UFUNCTION(BlueprintPure)
	UPlayerCore* GetPlayerCore() { return PlayerCore; };
};
