// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Common/PlayerCore.h"
#include "PlayerSlot.generated.h"


class AGamer;
class AGamerState;

/**
 * 
 */
UCLASS()
class LUDO_API APlayerSlot : public APlayerStart
{
	GENERATED_BODY()

	APlayerSlot(const FObjectInitializer& ObjectInitializer);

public:
	bool IsClaimed() const;

	bool TryClaim(AController* InController);

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	FPlayerCore PlayerCore;

private:
	void CheckUnclaimed();

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

public:
	const int8 GetIndex() const { return Index; };
	void SetIndex(int8 NewIndex) { Index = NewIndex; };

	const int8 GetPlayerOrder() const { return PlayOrder; };
	void SetPlayerOrder(int8 NewOrder) { PlayOrder = NewOrder; };

	AGamer* GetGamer() { return Gamer; };
	void SetGamer(AGamer* NewGamer) { Gamer = NewGamer; };

	AGamerState* GetGamerState();
};
