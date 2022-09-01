// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThrowWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class LUDO_API UThrowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Show(uint8 Number);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShow(uint8 Number);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DieNumberLabel;
};
