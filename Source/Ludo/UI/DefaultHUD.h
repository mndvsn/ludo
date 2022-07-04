// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefaultHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowMenuSignature);

/**
 * 
 */
UCLASS()
class LUDO_API UDefaultHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FShowMenuSignature OnShowMenu;
};
