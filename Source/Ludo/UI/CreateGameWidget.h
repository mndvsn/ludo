// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/MainMenuSubWidget.h"
#include "CreateGameWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LUDO_API UCreateGameWidget : public UMainMenuSubWidget
{
	GENERATED_BODY()

public:
	UCreateGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual EMainMenu GetEnum() override { return EMainMenu::MM_CreateGame; };

protected:
	void NativeOnInitialized() override;

	UFUNCTION()
	void ButtonCreateGameReleased();

	UFUNCTION()
	void ButtonBackReleased();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonCreateGame;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonBack;

	UPROPERTY(meta = (BindWidget))
	class USlider* SliderPlayersCPU;
};
