// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "UI/MainMenuSubWidget.h"
#include "ModeSelectWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LUDO_API UModeSelectWidget : public UMainMenuSubWidget
{
	GENERATED_BODY()

public:
	UModeSelectWidget(const FObjectInitializer& ObjectInitializer);

	virtual EMainMenu GetEnum() override { return EMainMenu::MM_ModeSelect; };

protected:
	void NativeOnInitialized() override;

	UFUNCTION()
	void ButtonGameCPUReleased();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonGameCPU;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonGameOnline;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonSettings;
};
