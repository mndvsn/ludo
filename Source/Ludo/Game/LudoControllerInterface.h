// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LudoControllerInterface.generated.h"


class AGamerState;
class AGamer;

UINTERFACE(MinimalAPI)
class ULudoControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LUDO_API ILudoControllerInterface
{
	GENERATED_BODY()

public:
	//UFUNCTION(Client, Reliable)
	virtual void Client_StartTurn() = 0;

	//UFUNCTION(Client, Reliable)
	virtual void Client_EndTurn() = 0;

	//UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_RequestEndTurn() = 0;

	virtual void ThrowDie() = 0;

	virtual TObjectPtr<AGamer> GetGamer() = 0;

	virtual TObjectPtr<AGamerState> GetGamerState() = 0;
};
