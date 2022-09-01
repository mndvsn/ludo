// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "Game/GameEventsInterface.h"
#include "Actors/PlayerEventsInterface.h"
#include "GameQuickMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowMenuSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTurnNameChanged, FString, PlayerName);

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
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
	void OnPlayerTurn(bool bIsPlayerTurn);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> LabelPlayerTurn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PlayerTurnBackground;

	virtual void NativeOnInitialized() override;

	void RemoveFromParent() override;

private:
	UPROPERTY()
	TScriptInterface<IPlayerEventsInterface> PlayerEventsInterface;

	UPROPERTY()
	TScriptInterface<IGameEventsInterface> GameEventsInterface;
	FDelegateHandle TurnChangedHandle;

	UFUNCTION()
	void OnTurnChange(uint8 NewPlayerIndex);

	UFUNCTION()
	void ButtonMenuReleased();
};
