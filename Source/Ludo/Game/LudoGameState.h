// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Actors/LudoEventComponent.h"
#include "LudoGameState.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ALudoGameState : public AGameStateBase
{
	GENERATED_BODY()

	ALudoGameState();

public:
	class AGamerState* GetGamerStateForIndex(int8 PlayerIndex) const;

	class AGamerState* GetGamerStateInTurn() const;

	bool IsPlayerTurn(APlayerController* Player);

	uint8 GetNumPlayersReady();

	void AdvanceTurn();

private:
	UPROPERTY(BlueprintGetter="GetEvents")
	ULudoEventComponent* EventComponent;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentPlayerIndex)
	int8 CurrentPlayerIndex = -1;

	UFUNCTION()
	void OnRep_CurrentPlayerIndex();

public:
	int8 GetCurrentPlayerIndex() { return CurrentPlayerIndex; };

	UFUNCTION(BlueprintPure)
	ULudoEventComponent* GetEvents() { return EventComponent; };
};
