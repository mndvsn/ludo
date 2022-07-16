// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actors/PlayerEventsInterface.h"
#include "GameActionsWidget.generated.h"


class UButton;

/**
 * 
 */
UCLASS()
class LUDO_API UGameActionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Events")
	void OnPlayerTurn(bool bIsPlayerTurn);

protected:
	void NativeOnInitialized() override;

	void RemoveFromParent() override;

	UPROPERTY()
	TScriptInterface<IPlayerEventsInterface> PlayerEventsInterface;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ButtonThrowDice;

	UFUNCTION(BlueprintNativeEvent, Category="Events")
	void OnButtonThrowDiceReleased();
};
