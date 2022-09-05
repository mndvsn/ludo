// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

#include "Game/GameEventsInterface.h"
#include "Game/LudoControllerInterface.h"
#include "Common/PlayerCore.h"
#include "LudoGameModeBase.generated.h"


class APlayerSlot;
class ALudoAIController;
class ABoard;
class APiece;
enum class EPlayState : uint8;

// Entry roll mask
UENUM(BlueprintType)
enum class EEntryRollMask : uint8 {
	ER_Low  UMETA(DisplayName = "Low"),
	ER_High UMETA(DisplayName = "High"),
	ER_All  UMETA(DisplayName = "All"),
};

/**
 * 
 */
UCLASS()
class LUDO_API ALudoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALudoGameModeBase();

	// Number of players set to play the game
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NumPlayers = 2;

	// Number of bots/cpu players
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NumPlayersCPU = 1;

	// Must be populated with FPlayerCores in BP
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPlayerCoreAsset* PlayerCores;

	// Knock off opponents pieces
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bKnockPieces = true;

	// Knock off multiple opponent pieces
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bKnockMultiple = true;

	// Entry + move on high roll
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bMoveOnHighEntryRoll = true;
	
	bool bShouldSpawnCPU = true;

protected:
	// Entry roll numbers
	TArray<uint8> EntryRollsLow = { 1 };
	TArray<uint8> EntryRollsHigh = { 6 };

public:
	TArray<uint8> GetEntryRolls(EEntryRollMask EntryRollMask) const;

	int32 GetNumPlayersTotal() const;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void GenericPlayerInitialization(AController* C) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void SetupPlayer(AController* Player);

	TArray<ALudoAIController*> CPUPlayers;

	ILudoControllerInterface* GetPlayerInTurn() const { return PlayerInTurn; }

	UFUNCTION(Category = "Events")
	void OnPlayStateChanged(AGamerState* GamerState, EPlayState State);

	UPROPERTY()
	TScriptInterface<IGameEventsInterface> GameEventsInterface;

	FDelegateHandle PlayStateChangedHandle;
	FDelegateHandle PlayerReachedGoalHandle;

private:
	TObjectPtr<ABoard> TheBoard;
	ILudoControllerInterface* PlayerInTurn;

	void CreatePlayerSlots(uint8 PlayerCount) const;

	void CreateCPUPlayers(uint8 NumCPUPlayers);

	void SpawnCPU();

	void SpawnPiecesForPlayer(TObjectPtr<AGamerState> GamerState) const;

	void SetupBoard();

	bool CheckGameReady() const;

	bool CheckGameFinished() const;

	void UpdateCurrentControllerState(bool bIsStartingTurn = true);
	
	void OnPlayerReachedGoal(const uint8 Player, const uint8 InGoalTotal);

	virtual void BeginDestroy() override;

public:
	TObjectPtr<ABoard> GetBoard() const { return TheBoard; }
	void SetBoard(TObjectPtr<ABoard> BoardActor);

	void StartGame();

	void AddPlayerThrow(FDieThrow Throw);

	void PlayerPieceReachedGoal(TObjectPtr<APiece> Piece) const;

	void NextTurn();

	void EndGame();

	virtual void BeginPlay() override;
};
