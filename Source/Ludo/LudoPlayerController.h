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

	int8 GetPlayerIndex() const;

	// RPCs

	UFUNCTION(Client, Reliable)
	void Client_StartTurn();

	UFUNCTION(Client, Reliable)
	void Client_EndTurn();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_RequestEndTurn();

	UFUNCTION(BlueprintPure)
	bool IsInTurn() { return InTurn; };

private:
	bool InTurn;
};
