// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamerState.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API AGamerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AGamerState();

	void UpdatePlayerName();

	void OnRep_PlayerName();
	UFUNCTION()
	void OnRep_PlayerIndex();

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerIndex)
	int8 PlayerIndex = -1;

public:
	int8 GetPlayerIndex() const { return PlayerIndex; }
	void SetPlayerIndex(int NewIndex);

	//virtual FString GetPlayerNameCustom() const;

protected:
	// Used to copy properties from the current PlayerState to the passed one
	virtual void CopyProperties(class APlayerState* PlayerState);
	
	// Used to override the current PlayerState with the properties of the passed one
	virtual void OverrideWith(class APlayerState* PlayerState);

};
