// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Game/GameEventsInterface.h"
#include "Common/PlayerCore.h"
#include "LudoGameState.generated.h"


class AGamerState;
class APlayerSlot;

/**
 *
 */
USTRUCT(BlueprintType)
struct FDieThrow
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 PlayerIndex;

	UPROPERTY()
	uint8 Result;
};

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

	bool IsPlayerTurn(TObjectPtr<APlayerController> Player) const;

	bool PlayerHasPieceOnBoard(int8 PlayerIndex) const;

	uint8 GetNumPlayersReady() const;
	uint8 GetNumPlayersReplicated() const;

	void AdvanceTurn();

private:
	FGE_OnTurnChangedNative OnTurnChangedNative;
	FGE_OnPlayerReachedGoalNative OnPlayerReachedGoalNative;
	FGE_OnPlayStateChangedNative OnPlayStateChangedNative;
	FGE_OnDieThrowNative OnDieThrowNative;

	UPROPERTY(Replicated)
	TArray<APlayerSlot*> PlayerSlots;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentPlayerIndex)
	int8 CurrentPlayerIndex = -1;

	UPROPERTY(Replicated)
	int8 WinnerPlayerIndex = -1;

	UPROPERTY(Replicated)
	uint8 PlayerCountForGame = 0;

	UPROPERTY(ReplicatedUsing=OnRep_DieThrowList)
	TArray<FDieThrow> DieThrowList;
	
	UPROPERTY(Replicated)
	TArray<uint8> PlayerPiecesInGoal;

	UFUNCTION()
	void OnRep_CurrentPlayerIndex();

	UFUNCTION()
	void OnRep_DieThrowList();

public:
	TArray<APlayerSlot*> GetPlayerSlots() const { return PlayerSlots; };
	void SetPlayerSlots(TArray<APlayerSlot*> InPlayerSlots);

	APlayerSlot* GetPlayerSlot(uint8 PlayerIndex) const;
	APlayerSlot* GetPlayerSlot(FPlayerCore PlayerCore) const;

	int8 GetCurrentPlayerIndex() const { return CurrentPlayerIndex; };

	uint8 GetWinner() const { return WinnerPlayerIndex; };

	bool HasGameEnded();

	uint8 GetPlayerCountForGame() const { return PlayerCountForGame; };
	void SetPlayerCountForGame(uint8 Count) { PlayerCountForGame = Count; };

	TArray<FDieThrow> GetThrows() const { return DieThrowList; };
	FDieThrow GetLastThrow() const { return GetThrows().Last(); };
	void AddDieThrow(FDieThrow Throw);

	void AddPlayerPieceInGoal(uint8 PlayerIndex);

	virtual FGE_OnTurnChangedNative& GetTurnChangedDelegate() override { return OnTurnChangedNative; };
	virtual FGE_OnPlayerReachedGoalNative& GetPlayerReachedGoalDelegate() override { return OnPlayerReachedGoalNative; };
	virtual FGE_OnPlayStateChangedNative& GetPlayStateChangedDelegate() override { return OnPlayStateChangedNative; };
	virtual FGE_OnDieThrowNative& GetDieThrowDelegate() override { return OnDieThrowNative; };
};
