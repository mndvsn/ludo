// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMenuHUD();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION(BlueprintCallable)
	void ConstructMenu();

	UFUNCTION(BlueprintCallable)
	void ShowMenu();

	UFUNCTION(BlueprintCallable)
	void HideMenu();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMainMenuWidget> MenuClass;

	UPROPERTY()
	class UUserWidget* Menu;
};
