// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameQuickMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowMenuSignature);

/**
 * 
 */
UCLASS()
class LUDO_API UGameQuickMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameQuickMenuWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FShowMenuSignature OnShowMenu;

protected:
	void NativeOnInitialized() override;

	void RemoveFromParent() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LabelPlayerTurn;

	UFUNCTION()
	void OnTurnChange(uint8 NewPlayerIndex);
};
