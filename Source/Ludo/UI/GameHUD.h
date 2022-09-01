// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "GameHUD.generated.h"


DECLARE_DELEGATE(FUIE_OnGameHUDReady);

class UGameOverlayWidget;
class UGameEndWidget;
class APlayerSlot;

/**
 * 
 */
UCLASS(Abstract)
class LUDO_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Show();

	UFUNCTION(BlueprintCallable)
	void Hide();

	UFUNCTION(BlueprintCallable)
	void ShowInGameMenu();

	UFUNCTION()
	void ShowEndScreen(APlayerSlot* WinnerSlot);

	FUIE_OnGameHUDReady OnGameHUDReady;

	UFUNCTION(BlueprintCallable, Category="Events")
	void OnFinishConstructHUD();

protected:
	UFUNCTION()
	void ConstructHUD();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameOverlayWidget> GameWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameEndWidget> GameEndWidgetClass;

	UPROPERTY()
	TObjectPtr<UGameOverlayWidget> GameWidget;

	UPROPERTY()
	TObjectPtr<UGameEndWidget> EndScreenWidget;
};
