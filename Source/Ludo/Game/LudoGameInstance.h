// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
	
	void CreateGameCPU(uint32 NumCPUPlayers = 3);

	void CreateGame();

	void EndGame();

	virtual void Shutdown() override;

private:
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
};
