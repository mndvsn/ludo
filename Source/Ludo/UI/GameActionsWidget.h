// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameActionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API UGameActionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameActionsWidget(const FObjectInitializer& ObjectInitializer);

protected:
	void NativeOnInitialized() override;

	void RemoveFromParent() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonThrowDice;

	UFUNCTION()
	void ButtonThrowDiceReleased();

	UFUNCTION()
	void OnPlayerTurn(bool IsPlayerTurn);

};
