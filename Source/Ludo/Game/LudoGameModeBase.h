// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LudoGameModeBase.generated.h"

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

	/*UPROPERTY(BlueprintReadOnly)
	TArray<class AGamer*> Players;*/

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	TArray<class ALudoPlayerStart*> AvailablePlayerStarts;

private:

	void CreatePlayerStarts(uint8 PlayerCount);

	void CheckGameReady();
};
