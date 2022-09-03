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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter=GetPlayerCore, ReplicatedUsing=OnRep_PlayerCore, Category="Player")
	FPlayerCore PlayerCore;

	UFUNCTION()
	virtual void OnRep_PlayerCore();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerCore();
	
public:
	UFUNCTION(BlueprintPure)
	bool IsHome() const { return bIsHome; };

	UFUNCTION(BlueprintPure)
	FPlayerCore GetPlayerCore() { return PlayerCore; };

	UFUNCTION()
	virtual void SetPlayerCore(FPlayerCore NewPlayerCore);
};
