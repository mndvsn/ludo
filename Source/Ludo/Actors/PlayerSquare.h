// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "Actors/Square.h"
#include "Common/PlayerCore.h"
#include "PlayerSquare.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class LUDO_API APlayerSquare : public ASquare
{
	GENERATED_BODY()
	
public:
	APlayerSquare();

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Square")
	bool bIsHome;
	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetPlayerCore, ReplicatedUsing=OnRep_PlayerCore, Category="Player")
	FPlayerCore PlayerCore;

	UFUNCTION()
	virtual void OnRep_PlayerCore();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerCore();
	
public:
	UFUNCTION(BlueprintPure)
	bool IsHome() const { return bIsHome; };

	UFUNCTION(BlueprintPure)
	const FPlayerCore& GetPlayerCore() { return PlayerCore; };

	UFUNCTION()
	virtual void SetPlayerCore(const FPlayerCore& NewPlayerCore);
};
