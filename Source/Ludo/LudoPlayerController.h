// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actors/LudoPlayerEventComponent.h"
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

private:
	bool bInTurn = false;

	UPROPERTY(BlueprintGetter = "GetEvents")
	ULudoPlayerEventComponent* PlayerEventComponent;

public:
	UFUNCTION(BlueprintPure)
	bool IsInTurn() { return bInTurn; };

	UFUNCTION(BlueprintPure)
	ULudoPlayerEventComponent* GetEvents() { return PlayerEventComponent; };

};
