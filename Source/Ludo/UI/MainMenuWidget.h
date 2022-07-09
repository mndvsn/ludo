// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API UMainMenuWidget : public UUserWidget, public IMainMenuInterface
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	void SetMainContent(UWidget* Content);

	// Menu interface
	virtual void NavigateMenu(EMainMenu ToMenu) override;
	virtual void ReturnToParentMenu() override;

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
	TSubclassOf<class UMainMenuSubWidget> MainContentClass;

	UPROPERTY()
	UMainMenuSubWidget* RootMenu;

	UPROPERTY(EditAnywhere, Category = "Content")
	TSubclassOf<class UMainMenuSubWidget> CreateGameWidgetClass;

	TArray<EMainMenu> MenuStack;

public:
	UFUNCTION(BlueprintPure)
	UUserWidget* GetMain() { return WidgetMain; };
	UFUNCTION(BlueprintPure)
	UNamedSlot* GetSlotMain() { return SlotMain; };
	UFUNCTION(BlueprintPure)
	UUserWidget* GetFooter() { return WidgetFooter; };
};
