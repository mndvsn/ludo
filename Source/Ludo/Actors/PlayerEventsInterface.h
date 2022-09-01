// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>

#include "PlayerEventsInterface.generated.h"


DECLARE_MULTICAST_DELEGATE(FPE_OnPlayerStateInitNative);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPE_OnPlayerTurn, bool, bIsPlayerTurn);

UINTERFACE(MinimalAPI)
class UPlayerEventsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LUDO_API IPlayerEventsInterface
{
	GENERATED_BODY()

public:
	virtual FPE_OnPlayerStateInitNative& GetPlayerStateInitDelegate() = 0;

	virtual FPE_OnPlayerTurn& GetPlayerTurnDelegate() = 0;
	
};
