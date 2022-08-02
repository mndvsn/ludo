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

	bool IsPlayerTurn(TObjectPtr<APlayerController> Player);

	bool PlayerHasPieceOnBoard(int8 PlayerIndex);

	uint8 GetNumPlayersReady();
	uint8 GetNumPlayersReplicated();

	void AdvanceTurn();

private:
	FGE_OnTurnChangedNative OnTurnChangedNative;
	FGE_OnPlayStateChangedNative OnPlayStateChangedNative;
	FGE_OnDieThrowNative OnDieThrowNative;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentPlayerIndex)
	int8 CurrentPlayerIndex = -1;

	UPROPERTY(Replicated)
	uint8 PlayerCountForGame = 0;

	UPROPERTY(ReplicatedUsing=OnRep_DieThrowList)
	TArray<FDieThrow> DieThrowList;
	
	UFUNCTION()
	void OnRep_CurrentPlayerIndex();

	UFUNCTION()
	void OnRep_DieThrowList();

public:
	int8 GetCurrentPlayerIndex() { return CurrentPlayerIndex; };

	uint8 GetPlayerCountForGame() { return PlayerCountForGame; };
	void SetPlayerCountForGame(uint8 Count) { PlayerCountForGame = Count; };

	TArray<FDieThrow> GetThrows() { return DieThrowList; };
	FDieThrow GetLastThrow() { return GetThrows().Last(); };
	void AddDieThrow(FDieThrow Throw);

	virtual FGE_OnTurnChangedNative& GetTurnChangedDelegate() override { return OnTurnChangedNative; };
	virtual FGE_OnPlayStateChangedNative& GetPlayStateChangedDelegate() override { return OnPlayStateChangedNative; };
	virtual FGE_OnDieThrowNative& GetDieThrowDelegate() override { return OnDieThrowNative; };
};
