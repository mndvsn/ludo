// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "UI/MainMenuSubWidget.h"
#include "CreateGameWidget.generated.h"


class UEditableTextBox;
class UButton;
class USlider;

UCLASS(Abstract)
class LUDO_API UCreateGameWidget : public UMainMenuSubWidget
{
	GENERATED_BODY()

public:
	UCreateGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual EMainMenu GetEnum() override { return EMainMenu::MM_CreateGame; };

	UPROPERTY(BlueprintReadOnly, Category="Navigation", meta=(BindWidget))
	UButton* ButtonCreateGame;

	UPROPERTY(BlueprintReadOnly, Category="Navigation", meta=(BindWidget))
	UButton* ButtonBack;

	UPROPERTY(BlueprintReadOnly, Category="Input", meta=(BindWidget))
	USlider* SliderPlayersCPU;

	UPROPERTY(BlueprintReadOnly, Category="Input", meta=(BindWidget))
	UEditableTextBox* TextBoxSeed;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void ButtonCreateGameReleased();

	UFUNCTION()
	void ButtonBackReleased();
};
