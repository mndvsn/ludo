// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "GameEndWidget.generated.h"


class APlayerSlot;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS(Abstract)
class LUDO_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerWinLoseLabel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> WinnerLabel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> ButtonReturn;

	UFUNCTION()
	void SetValues(const APlayerSlot* WinnerSlot) const;
};
