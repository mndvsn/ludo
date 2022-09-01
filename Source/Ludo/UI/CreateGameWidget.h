// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "UI/MainMenuSubWidget.h"
#include "CreateGameWidget.generated.h"


class UButton;
class USlider;

UCLASS(Abstract)
class LUDO_API UCreateGameWidget : public UMainMenuSubWidget
{
	GENERATED_BODY()

public:
	UCreateGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual EMainMenu GetEnum() override { return EMainMenu::MM_CreateGame; };

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void ButtonCreateGameReleased();

	UFUNCTION()
	void ButtonBackReleased();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonCreateGame;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBack;

	UPROPERTY(meta = (BindWidget))
	USlider* SliderPlayersCPU;
};
