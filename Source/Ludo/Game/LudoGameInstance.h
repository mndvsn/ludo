// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Engine/GameInstance.h>

#include "Game/GameSettings.h"
#include "LudoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ULudoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void CreateGame(const uint32 InRandomSeed, const uint8 InPlayers = 4, const uint8 InCPU = 3);

	void EndGame();

	virtual void Shutdown() override;

private:
	FGameSettings TempGameSettings;

	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);

public:
	FGameSettings GetTempGameSettings() const { return TempGameSettings; }
	void StoreTempGameSettings(const FGameSettings& TempSettings) { TempGameSettings = TempSettings; }
};
