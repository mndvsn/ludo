// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamerState.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API AGamerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AGamerState();

	void OnRep_PlayerName();
	
private:

	void UpdatePlayerName();

};
