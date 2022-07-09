// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LudoGamerInterface.h"
#include "LudoGameModeBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogLudoGM, Verbose, All);

enum class EPlayState : uint8;

/**
 * 
 */
UCLASS()
class LUDO_API ALudoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALudoGameModeBase();

	// Number of players set to play the game, default 2
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NumPlayers = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NumPlayersCPU = 0;

	bool bShouldSpawnCPU = 0;

	int32 GetNumPlayersTotal();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void GenericPlayerInitialization(AController* C) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void SetupPlayer(AController* Player);

	TArray<class ALudoPlayerStart*> PlayerStarts;

	TArray<class ALudoAIController*> CPUPlayers;

	ILudoGamerInterface* GetPlayerInTurn() { return PlayerInTurn; };

	UFUNCTION()
	void OnPlayStateChange(class AGamerState* GamerState, EPlayState State);

private:
	ILudoGamerInterface* PlayerInTurn;

	void CreatePlayerStarts(uint8 PlayerCount);

	void CreateCPUPlayers(uint8 NumCPUPlayers);

	void SpawnCPU();

	bool CheckGameReady();

	void UpdateCurrentControllerState(bool bIsStartingTurn = true);

public:
	void StartGame();

	void NextTurn();

};
