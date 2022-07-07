// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "LudoPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ALudoPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	const uint8 GetPlayerSlot() const { return PlayerSlot; };

	void SetPlayerSlot(uint8 SlotIndex);

	bool IsClaimed() const { return Claimed; };

	bool TryClaim(AController* InController);

protected:
	void CheckUnclaimed();

	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	UPROPERTY()
	bool Claimed = false;

	UPROPERTY()
	uint8 PlayerSlot = -1;
};
