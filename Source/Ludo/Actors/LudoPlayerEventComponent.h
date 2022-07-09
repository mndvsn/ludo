// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LudoPlayerEventComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerEventDelegate_OnPlayerTurn, bool, IsPlayerTurn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerEventDelegate_OnPlayerStateInit);


UCLASS(ClassGroup = (Custom), meta=(BlueprintSpawnableComponent), Category="Player Events")
class LUDO_API ULudoPlayerEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULudoPlayerEventComponent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Player Events")
	FPlayerEventDelegate_OnPlayerTurn OnPlayerTurn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Player Events")
	FPlayerEventDelegate_OnPlayerStateInit OnPlayerStateInit;
};
