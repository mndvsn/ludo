// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/GameEventsInterface.h"
#include "Actors/PlayerEventsInterface.h"
#include "LudoGamerInterface.h"
#include "LudoPlayerController.generated.h"


class AGamerState;
class ABoard;

/**
 * 
 */
UCLASS()
class LUDO_API ALudoPlayerController : public APlayerController, public ILudoGamerInterface, public IPlayerEventsInterface
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

	void CheckPlayerStates();

	virtual TObjectPtr<AGamerState> GetGamerState() override;

	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameHUDReady();

	TScriptInterface<IGameEventsInterface> GameEventsInterface;

	FDelegateHandle PlayStateChangedHandle;

private:
	bool bClientReady = false;
	bool bInTurn = false;

	FPE_OnPlayerStateInitNative OnPlayerStateInitNative;
	FPE_OnPlayerTurn OnPlayerTurn;

public:
	UPROPERTY()
	TObjectPtr<ABoard> TheBoard;

	UFUNCTION(BlueprintPure)
	bool IsInTurn() { return bInTurn; };

	void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;

	void SetGameEventsInterface(TScriptInterface<IGameEventsInterface> Interface) { GameEventsInterface = Interface; };

	virtual FPE_OnPlayerStateInitNative& GetPlayerStateInitDelegate() override { return OnPlayerStateInitNative; };
	virtual FPE_OnPlayerTurn& GetPlayerTurnDelegate() override { return OnPlayerTurn; };
};
