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

	UFUNCTION(BlueprintCallable)
	void ConstructMenu();

	UFUNCTION(BlueprintCallable)
	void ShowMenu();

	UFUNCTION(BlueprintCallable)
	void HideMenu();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool MenuIsValid();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> MenuClass;

	UPROPERTY()
	class UUserWidget* Menu;
};
