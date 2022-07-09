// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/GamerState.h"
#include "LudoEventComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameEventDelegate_OnTurnChanged, uint8, NewPlayerIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameEventDelegate_OnPlayStateChanged, AGamerState*, GamerState, EPlayState, State);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Category="Global Events")
class LUDO_API ULudoEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULudoEventComponent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game Events")
	FGameEventDelegate_OnTurnChanged OnTurnChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game Events")
	FGameEventDelegate_OnPlayStateChanged OnPlayStateChange;
};
