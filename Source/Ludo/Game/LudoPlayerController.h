// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerController.h>

#include "Game/GameEventsInterface.h"
#include "Game/LudoControllerInterface.h"
#include "Game/LudoGameState.h"
#include "Actors/PlayerEventsInterface.h"
#include "LudoPlayerController.generated.h"


class APlayerState;
class AGamerState;
class ABoard;
class AGamer;

/**
 * 
 */
UCLASS()
class LUDO_API ALudoPlayerController : public APlayerController, public ILudoControllerInterface, public IPlayerEventsInterface
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

	virtual void ThrowDie() override;

	/*UFUNCTION(NetMulticast, Reliable)
	void Client_OnDieThrown(FDieThrow Throw);*/

	UFUNCTION(Server, Reliable)
	void Server_NotifyOnReady(APlayerState* PlayerStateReady);

	UFUNCTION(Server, Reliable)
	void Server_ShowEndScreen();

	void CheckPlayerStates();

	virtual TObjectPtr<AGamerState> GetGamerState() override;

	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameHUDReady();

	TObjectPtr<ABoard> GameBoard;
	
	TScriptInterface<IGameEventsInterface> GameEventsInterface;
	FDelegateHandle PlayStateChangedHandle;

private:
	bool bClientReady = false;
	bool bInTurn = false;

	FPE_OnPlayerStateInitNative OnPlayerStateInitNative;
	FPE_OnPlayerTurn OnPlayerTurn;

public:
	UFUNCTION(BlueprintPure)
	ABoard* GetBoard() const { return GameBoard; }

	virtual TObjectPtr<AGamer> GetGamer() override;

	UFUNCTION(BlueprintPure)
	bool IsInTurn() const { return bInTurn; };

	void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;

	void SetGameEventsInterface(TScriptInterface<IGameEventsInterface> Interface) { GameEventsInterface = Interface; };

	virtual FPE_OnPlayerStateInitNative& GetPlayerStateInitDelegate() override { return OnPlayerStateInitNative; };
	virtual FPE_OnPlayerTurn& GetPlayerTurnDelegate() override { return OnPlayerTurn; };
};
