// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	void NativeOnInitialized() override;

	UFUNCTION()
	void ButtonExitGameReleased();

private:
	UPROPERTY(meta = (BindWidget), BlueprintGetter="GetMain")
	UUserWidget* WidgetMain;

	UPROPERTY(meta = (BindWidget), BlueprintGetter="GetContents")
	UUserWidget* WidgetMainContents;

	UPROPERTY(meta = (BindWidget), BlueprintGetter="GetFooter")
	UUserWidget* WidgetFooter;

	class UButton* ButtonExitGame;

public:
	UFUNCTION(BlueprintPure)
	UUserWidget* GetMain() { return WidgetMain; };
	UFUNCTION(BlueprintPure)
	UUserWidget* GetContents() { return WidgetMainContents; };
	UFUNCTION(BlueprintPure)
	UUserWidget* GetFooter() { return WidgetFooter; };
};
