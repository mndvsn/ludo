// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LudoGameModeBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogLudoGM, Verbose, All);
/**
 * 
 */
UCLASS()
class LUDO_API ALudoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALudoGameModeBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumberOfPlayers;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	TArray<class ALudoPlayerStart*> PlayerStarts;

	class ALudoPlayerController* GetPlayerControllerInTurn() { return PlayerControllerInTurn; };

private:
	class ALudoPlayerController* PlayerControllerInTurn;

	void CreatePlayerStarts(uint8 PlayerCount);

	bool CheckGameReady();

	void StartGame();

public:
	void NextTurn();
};
