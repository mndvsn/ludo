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

	UFUNCTION(BlueprintNativeEvent, Category = "Events")
	void OnPlayerTurn(bool IsPlayerTurn);

protected:
	void NativeOnInitialized() override;

	void RemoveFromParent() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ButtonThrowDice;

	UFUNCTION(BlueprintNativeEvent, Category="Events")
	void OnButtonThrowDiceReleased();

};
