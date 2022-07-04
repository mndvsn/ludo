// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LudoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ALudoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALudoPlayerController();

	int GetPlayerIndex() const { return PlayerIndex; }
	void SetPlayerIndex(int val);

private:
	int PlayerIndex;
};
