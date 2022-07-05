// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "LudoPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ALudoPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	const uint8 GetPlayerIndex() const { return PlayerIndex; };

	UFUNCTION(BlueprintCallable)
	void SetPlayerIndex(uint8 Index);

private:
	UPROPERTY(BlueprintGetter="GetPlayerIndex")
	uint8 PlayerIndex;
};
