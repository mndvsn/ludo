// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Engine/GameInstance.h>

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
	
	void CreateGameCPU(const uint32 InRandomSeed, const uint8 InPlayers = 4, const uint8 InCPU = 3) const;

	void CreateGame(const uint32 InRandomSeed, const uint8 InPlayers, const uint8 InCPU = 0) const;

	void EndGame();

	virtual void Shutdown() override;

private:
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
};
