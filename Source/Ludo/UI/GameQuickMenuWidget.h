// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameQuickMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowMenuSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTurnNameChanged, FString, PlayerName);

/**
 * 
 */
UCLASS()
class LUDO_API UGameQuickMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FShowMenuSignature OnShowMenu;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FPlayerTurnNameChanged OnPlayerTurnNameChanged;

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Events")
	void OnPlayerTurn(bool IsPlayerTurn);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* LabelPlayerTurn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* PlayerTurnBackground;

	virtual void NativeOnInitialized() override;

	void RemoveFromParent() override;

private:
	UFUNCTION()
	void OnTurnChange(uint8 NewPlayerIndex);

	UFUNCTION()
	void ButtonMenuReleased();
};
