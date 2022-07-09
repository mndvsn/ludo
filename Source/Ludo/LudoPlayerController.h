// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actors/LudoPlayerEventComponent.h"
#include "LudoGamerInterface.h"
#include "LudoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ALudoPlayerController : public APlayerController, public ILudoGamerInterface
{
	GENERATED_BODY()

public:
	ALudoPlayerController();

	int8 GetPlayerIndex() const;

	// RPCs

	UFUNCTION(Client, Reliable)
	virtual void Client_StartTurn() override;

	UFUNCTION(Client, Reliable)
	virtual void Client_EndTurn() override;

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_RequestEndTurn() override;

	UFUNCTION(Server, Reliable)
	void Server_NotifyOnReady(class APlayerState* PlayerStateReady);

	virtual class AGamerState* GetGamerState() override;

	virtual void OnRep_PlayerState() override;

private:
	bool bInTurn = false;
	bool bClientReadyOnPlayerState = true;

	UPROPERTY(BlueprintGetter = "GetEvents")
	ULudoPlayerEventComponent* PlayerEventComponent;

public:
	UFUNCTION(BlueprintPure)
	bool IsInTurn() { return bInTurn; };

	void SetClientReadyOnPlayerState(bool bInValue) { bClientReadyOnPlayerState = bInValue; };

	UFUNCTION(BlueprintPure)
	ULudoPlayerEventComponent* GetEvents() { return PlayerEventComponent; };
};
