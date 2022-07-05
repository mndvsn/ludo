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

	ULudoGameInstance();

public:
	void Init() override;

	void CreateGame();

	void EndGame();

private:
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
};
