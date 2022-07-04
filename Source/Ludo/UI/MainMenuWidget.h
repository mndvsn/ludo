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

	void SetMainContent(UWidget* Content);

protected:
	void NativeOnInitialized() override;

	UFUNCTION()
	void ButtonExitGameReleased();

private:
	UPROPERTY(meta = (BindWidget), BlueprintGetter="GetMain")
	UUserWidget* WidgetMain;

	UPROPERTY(meta = (BindWidget), BlueprintGetter="GetSlotMain")
	class UNamedSlot* SlotMain;

	UPROPERTY(meta = (BindWidget), BlueprintGetter="GetFooter")
	UUserWidget* WidgetFooter;

	class UButton* ButtonExitGame;

	UPROPERTY(EditAnywhere, Category="Content")
	TSubclassOf<UUserWidget> DefaultMainContent_Class;
	UUserWidget* RootMenu;

public:
	UFUNCTION(BlueprintPure)
	UUserWidget* GetMain() { return WidgetMain; };
	UFUNCTION(BlueprintPure)
	UNamedSlot* GetSlotMain() { return SlotMain; };
	UFUNCTION(BlueprintPure)
	UUserWidget* GetFooter() { return WidgetFooter; };
};
