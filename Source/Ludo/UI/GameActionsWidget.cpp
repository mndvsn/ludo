// Copyright Martin Furuberg. All Rights Reserved.


#include "GameActionsWidget.h"
#include "Components/Button.h"

#include "LudoPlayerController.h"


UGameActionsWidget::UGameActionsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UGameActionsWidget::NativeOnInitialized()
{
	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		PC->GetEvents()->OnPlayerTurn.AddDynamic(this, &UGameActionsWidget::OnPlayerTurn);
	}

	if (ButtonThrowDice)
	{
		ButtonThrowDice->OnReleased.AddDynamic(this, &UGameActionsWidget::ButtonThrowDiceReleased);
		ButtonThrowDice->SetIsEnabled(false);
	}
}

void UGameActionsWidget::RemoveFromParent()
{
	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		PC->GetEvents()->OnPlayerTurn.RemoveDynamic(this, &UGameActionsWidget::OnPlayerTurn);
	}

	if (ButtonThrowDice)
	{
		ButtonThrowDice->OnReleased.RemoveDynamic(this, &UGameActionsWidget::ButtonThrowDiceReleased);
	}

	Super::RemoveFromParent();
}

void UGameActionsWidget::ButtonThrowDiceReleased()
{
	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		if (PC->IsInTurn())
		{
			PC->Server_RequestEndTurn();
		}
	}
}

void UGameActionsWidget::OnPlayerTurn(bool IsPlayerTurn)
{
	if (!ButtonThrowDice) return;
	ButtonThrowDice->SetIsEnabled(IsPlayerTurn);
}
