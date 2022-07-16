// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Game/GameEventsInterface.h"
#include "LudoGameState.generated.h"


class AGamerState;

/**
 * 
 */
UCLASS()
class LUDO_API ALudoGameState : public AGameStateBase, public IGameEventsInterface
{
	GENERATED_BODY()

public:
	TObjectPtr<AGamerState> GetGamerStateForIndex(int8 PlayerIndex) const;

	TObjectPtr<AGamerState> GetGamerStateInTurn() const;

	bool IsPlayerTurn(TObjectPtr<APlayerController> Player);

	uint8 GetNumPlayersReady();
	uint8 GetNumPlayersReplicated();

	void AdvanceTurn();

private:
	FGE_OnTurnChangedNative OnTurnChangedNative;
	FGE_OnPlayStateChangedNative OnPlayStateChangedNative;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentPlayerIndex)
	int8 CurrentPlayerIndex = -1;

	UPROPERTY(Replicated)
	uint8 PlayerCountForGame = 0;

	UFUNCTION()
	void OnRep_CurrentPlayerIndex();

public:
	int8 GetCurrentPlayerIndex() { return CurrentPlayerIndex; };

	uint8 GetPlayerCountForGame() { return PlayerCountForGame; };
	void SetPlayerCountForGame(uint8 Count) { PlayerCountForGame = Count; };

	virtual FGE_OnTurnChangedNative& GetTurnChangedDelegate() override { return OnTurnChangedNative; };
	virtual FGE_OnPlayStateChangedNative& GetPlayStateChangedDelegate() override { return OnPlayStateChangedNative; };
};
