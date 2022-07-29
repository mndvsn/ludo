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

	virtual void Client_StartTurn() override;

	virtual void Client_EndTurn() override;

	virtual void Server_ThrowDie() override;

	virtual TObjectPtr<AGamerState> GetGamerState() override;

protected:
	void Process();

	UFUNCTION()
	void OnWaited();

private:
	bool bInTurn;

	FTimerHandle WaitTimer;

};
