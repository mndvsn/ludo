// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LudoEventComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameEventDelegate_OnTurnChange, uint8, NewPlayerIndex);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Category="Global Events")
class LUDO_API ULudoEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULudoEventComponent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game Events")
	FGameEventDelegate_OnTurnChange OnTurnChange;
};
