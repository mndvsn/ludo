// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerStart.h>

#include "Common/PlayerCore.h"
#include "PlayerSlot.generated.h"


class AGamer;
class AGamerState;

/**
 * 
 */
UCLASS(NotPlaceable)
class LUDO_API APlayerSlot : public APlayerStart
{
	GENERATED_BODY()

	APlayerSlot(const FObjectInitializer& ObjectInitializer);

public:
	bool IsClaimed() const;

	bool TryClaim(AController* InController);

private:
	UPROPERTY(Replicated, BlueprintGetter=GetPlayerCore)
	FPlayerCore PlayerCore;
	
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	UPROPERTY(Replicated)
	bool bClaimed = false;

	UPROPERTY(Replicated)
	int8 Index = -1;

	UPROPERTY(Replicated)
	uint8 PlayOrder;

	UPROPERTY(Replicated)
	TObjectPtr<AGamer> Gamer;
	
	void CheckUnclaimed();

public:
	int8 GetIndex() const { return Index; }
	void SetIndex(const int8 NewIndex) { Index = NewIndex; }

	UFUNCTION(BlueprintPure)
	const FPlayerCore& GetPlayerCore() const { return PlayerCore; }
	void SetPlayerCore(const FPlayerCore& NewPlayerCore) { PlayerCore = NewPlayerCore; }

	int8 GetPlayerOrder() const { return PlayOrder; }
	void SetPlayerOrder(const int8 NewOrder) { PlayOrder = NewOrder; }

	AGamer* GetGamer() const { return Gamer; }
	void SetGamer(AGamer* NewGamer) { Gamer = NewGamer; }

	AGamerState* GetGamerState() const;
};
