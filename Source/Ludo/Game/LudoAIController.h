// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LudoGamerInterface.h"
#include "LudoAIController.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API ALudoAIController : public AAIController, public ILudoGamerInterface
{
	GENERATED_BODY()

public:
	ALudoAIController(const FObjectInitializer& ObjectInitializer);

	// RPCs

	//UFUNCTION(Client, Reliable)
	virtual void Client_StartTurn() override;

	//UFUNCTION(Client, Reliable)
	virtual void Client_EndTurn() override;

	//UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_RequestEndTurn() override;

	virtual AGamerState* GetGamerState() override;

private:
	bool bInTurn;

};
