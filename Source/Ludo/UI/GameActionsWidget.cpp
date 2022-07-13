// Copyright Martin Furuberg. All Rights Reserved.


#include "GameActionsWidget.h"
#include "Components/Button.h"
#include "Game/GamerState.h"

#include "Actors/Board.h"

#include "LudoPlayerController.h"


UGameActionsWidget::UGameActionsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UGameActionsWidget::NativeOnInitialized()
{
	if (ButtonThrowDice)
	{
		ButtonThrowDice->OnReleased.AddDynamic(this, &UGameActionsWidget::OnButtonThrowDiceReleased);
		ButtonThrowDice->SetIsEnabled(false);
	}

	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		PC->GetEvents()->OnPlayerTurn.AddDynamic(this, &UGameActionsWidget::OnPlayerTurn);

		// Notify server on loaded/ready
		if (AGamerState* GamerState = PC->GetPlayerState<AGamerState>())
		{
			PC->Server_NotifyOnReady(GamerState);
		}
		else
		{
			// Replication takes more time, set flag in PC
			PC->SetClientReadyOnPlayerState(true);
		}
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
		ButtonThrowDice->OnReleased.RemoveDynamic(this, &UGameActionsWidget::OnButtonThrowDiceReleased);
	}

	Super::RemoveFromParent();
}

void UGameActionsWidget::OnButtonThrowDiceReleased_Implementation()
{
	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		/*if (PC->IsInTurn())
		{
			PC->Server_RequestEndTurn();
		}*/

		PC->TheBoard->Search(FMath::RandRange(0,39), FMath::RandRange(0, 20));
	}
}

void UGameActionsWidget::OnPlayerTurn_Implementation(bool IsPlayerTurn)
{
	
}
