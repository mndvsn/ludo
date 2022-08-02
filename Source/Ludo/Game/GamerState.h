// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamerState.generated.h"


UENUM(BlueprintType)
enum class EPlayState : uint8
{
	Transitioning  UMETA(DisplayName = "Transitioning"),
	Ready  UMETA(DisplayName = "Ready"),
	Playing  UMETA(DisplayName = "Playing"),
	Waiting  UMETA(DisplayName = "Waiting"),
};

class UPlayerCore;
class APlayerState;

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

	UPROPERTY(Replicated)
	EPlayState PlayState;

	UPROPERTY(Replicated)
	TObjectPtr<UPlayerCore> PlayerCore;

public:
	int8 GetPlayerIndex() const { return PlayerIndex; }
	void SetPlayerIndex(int NewIndex);

	TObjectPtr<UPlayerCore> GetPlayerCore() const { return PlayerCore; }
	void SetPlayerCore(TObjectPtr<UPlayerCore> NewCore);

	EPlayState GetPlayState() const { return PlayState; }
	void SetPlayState(EPlayState State) { PlayState = State; };

	//virtual FString GetPlayerNameCustom() const;

protected:
	// Used to copy properties from the current PlayerState to the passed one
	virtual void CopyProperties(APlayerState* PlayerState);
	
	// Used to override the current PlayerState with the properties of the passed one
	virtual void OverrideWith(APlayerState* PlayerState);

};
