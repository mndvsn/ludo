// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter=GetPlayerCore, Category="Square")
	FPlayerCore PlayerCore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Square")
	bool bIsHome;

public:
	UFUNCTION(BlueprintPure)
	bool IsHome() const { return bIsHome; };

	UFUNCTION(BlueprintPure)
	FPlayerCore GetPlayerCore() { return PlayerCore; };

	UFUNCTION(BlueprintNativeEvent)
	void SetPlayerCore(FPlayerCore NewPlayerCore);
};
