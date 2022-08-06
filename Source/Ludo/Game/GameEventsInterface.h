// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/GamerState.h"
#include "GameEventsInterface.generated.h"


struct FDieThrow;

DECLARE_MULTICAST_DELEGATE_OneParam(FGE_OnTurnChangedNative,/* PlayerIndex */ uint8);

DECLARE_MULTICAST_DELEGATE_TwoParams(FGE_OnPlayerReachedGoalNative,/* PlayerIndex */ uint8,/* InGoalTotal */ uint8);

DECLARE_MULTICAST_DELEGATE_TwoParams(FGE_OnPlayStateChangedNative, AGamerState*, EPlayState);

DECLARE_MULTICAST_DELEGATE_OneParam(FGE_OnDieThrowNative, FDieThrow);

UINTERFACE(MinimalAPI)
class UGameEventsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LUDO_API IGameEventsInterface
{
	GENERATED_BODY()

public:
	virtual FGE_OnTurnChangedNative& GetTurnChangedDelegate() = 0;

	virtual FGE_OnPlayerReachedGoalNative& GetPlayerReachedGoalDelegate() = 0;

	virtual FGE_OnPlayStateChangedNative& GetPlayStateChangedDelegate() = 0;
	
	virtual FGE_OnDieThrowNative& GetDieThrowDelegate() = 0;
};
