// Copyright Martin Furuberg. All Rights Reserved.


#include "GameActionsWidget.h"
#include "Components/Button.h"

#include "Game/GamerState.h"
#include "LudoPlayerController.h"
#include "Actors/Board.h"


void UGameActionsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonThrowDice)
	{
		ButtonThrowDice->OnReleased.AddDynamic(this, &UGameActionsWidget::OnButtonThrowDiceReleased);
		ButtonThrowDice->SetIsEnabled(false);
	}

	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		PlayerEventsInterface = PC;

		if (PlayerEventsInterface)
		{
			PlayerEventsInterface->GetPlayerTurnDelegate().AddDynamic(this, &UGameActionsWidget::OnPlayerTurn);
		}
	}
}

void UGameActionsWidget::RemoveFromParent()
{
	if (PlayerEventsInterface)
	{
		PlayerEventsInterface->GetPlayerTurnDelegate().RemoveDynamic(this, &UGameActionsWidget::OnPlayerTurn);

		PlayerEventsInterface = nullptr;
	}
	
	if (ButtonThrowDice)
	{
		ButtonThrowDice->OnReleased.RemoveDynamic(this, &UGameActionsWidget::OnButtonThrowDiceReleased);
	}

	Super::RemoveFromParent();
}

void UGameActionsWidget::OnButtonThrowDiceReleased_Implementation()
{
	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		if (PC->IsInTurn())
		{
			PC->Server_RequestEndTurn();
		}

		//PC->TheBoard->Search(FMath::RandRange(0,39), FMath::RandRange(0, 20));
	}
}

void UGameActionsWidget::OnPlayerTurn_Implementation(bool bIsPlayerTurn)
{

}
