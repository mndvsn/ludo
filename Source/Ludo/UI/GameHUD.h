// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
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

	UFUNCTION()
	void ShowInGameMenu();

protected:
	UFUNCTION()
	void ConstructHUD();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameOverlayWidget> GameWidgetClass;

	UPROPERTY()
	class UGameOverlayWidget* GameWidget;
};
