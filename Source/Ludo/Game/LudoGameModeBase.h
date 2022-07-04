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

	/*virtual void BeginPlay() override;
	virtual void StartPlay() override;*/
	
	//TArray<class APlayerStart*> GetAvailableStartPoints();
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	//virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	
	// start positions
	TArray<class APlayerStart*> AvailableStartPoints;
private:

};
