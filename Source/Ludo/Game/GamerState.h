// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerState.h>

#include "GamerState.generated.h"


UENUM(BlueprintType)
enum class EPlayState : uint8
{
	Transitioning  UMETA(DisplayName = "Transitioning"),
	Ready  UMETA(DisplayName = "Ready"),
	Playing  UMETA(DisplayName = "Playing"),
	Waiting  UMETA(DisplayName = "Waiting"),
};

class APlayerState;
class APlayerSlot;

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

	virtual void OnRep_PlayerName() override;
	UFUNCTION()
	void OnRep_PlayerIndex() const;

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerIndex)
	int8 PlayerIndex = -1;

	UPROPERTY(Replicated)
	EPlayState PlayState;

	UPROPERTY(Replicated)
	TObjectPtr<APlayerSlot> PlayerSlot;

public:
	int8 GetPlayerIndex() const { return PlayerIndex; }
	void SetPlayerIndex(const uint8 NewIndex);

	TObjectPtr<APlayerSlot> GetPlayerSlot() const { return PlayerSlot; }
	void SetPlayerSlot(const TObjectPtr<APlayerSlot> NewSlot);

	EPlayState GetPlayState() const { return PlayState; }
	void SetPlayState(const EPlayState State) { PlayState = State; }

	//virtual FString GetPlayerNameCustom() const;

protected:
	// Used to copy properties from the current PlayerState to the passed one
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	// Used to override the current PlayerState with the properties of the passed one
	virtual void OverrideWith(APlayerState* PlayerState) override;
};
