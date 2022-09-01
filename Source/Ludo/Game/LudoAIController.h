// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <AIController.h>

#include "Game/LudoControllerInterface.h"
#include "LudoAIController.generated.h"

class AGamer;
class AGamerState;

/**
 * 
 */
UCLASS()
class LUDO_API ALudoAIController : public AAIController, public ILudoControllerInterface
{
	GENERATED_BODY()

public:
	ALudoAIController(const FObjectInitializer& ObjectInitializer);

	virtual void Client_StartTurn() override;

	virtual void Client_EndTurn() override;

	virtual void Server_RequestEndTurn() override;

	virtual void ThrowDie() override;

	virtual TObjectPtr<AGamer> GetGamer() override;

	virtual TObjectPtr<AGamerState> GetGamerState() override;

protected:
	void Process();

	UFUNCTION()
	void OnWaited();

private:
	bool bInTurn;

	FTimerHandle WaitTimer;

};
