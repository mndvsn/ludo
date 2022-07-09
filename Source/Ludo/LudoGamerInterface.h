// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LudoGamerInterface.generated.h"

UINTERFACE(MinimalAPI)
class ULudoGamerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LUDO_API ILudoGamerInterface
{
	GENERATED_BODY()

public:
	//UFUNCTION(Client, Reliable)
	virtual void Client_StartTurn() = 0;

	//UFUNCTION(Client, Reliable)
	virtual void Client_EndTurn() = 0;

	//UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_RequestEndTurn() = 0;

};
