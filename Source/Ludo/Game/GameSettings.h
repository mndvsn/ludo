// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "GameSettings.generated.h"


USTRUCT(BlueprintType)
struct FGameSettings
{
	GENERATED_BODY()

	// Random Stream seed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Seed = 0;
	
	// Number of players set to play the game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 NumPlayers = 2;

	// Number of bots/cpu players
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 NumPlayersCPU = 1;

	// Knock off opponents pieces
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bKnockPieces = true;

	// Knock off multiple opponent pieces
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bKnockMultiple = true;

	// Entry + move on high roll
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMoveOnHighEntryRoll = true;
};
