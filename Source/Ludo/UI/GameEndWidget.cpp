// Copyright Martin Furuberg. All Rights Reserved.


#include "UI/GameEndWidget.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

#include "Game/GamerState.h"
#include "Game/LudoPlayerController.h"
#include "Actors/Gamer.h"
#include "Actors/PlayerSlot.h"


void UGameEndWidget::SetValues(const APlayerSlot* WinnerSlot) const
{
	if (!WinnerSlot) return;

	const FString PlayerName = WinnerSlot->GetGamerState()->GetPlayerName();

	// Get PlayerSlot from controlled Gamer
	const TObjectPtr<ALudoPlayerController> PC = GetOwningPlayer<ALudoPlayerController>();
	if (const APlayerSlot* PlayerSlot = PC->GetGamer()->GetPlayerSlot())
	{
		// Set result of game
		const FString PlayerResult = WinnerSlot->GetPlayerCore() == PlayerSlot->GetPlayerCore() ? TEXT("Winner!") : TEXT("Lost!");
		PlayerWinLoseLabel->SetText(FText::FromString(PlayerResult));
	}
	
	WinnerLabel->SetText(FText::FromString(PlayerName));
	WinnerLabel->SetColorAndOpacity(WinnerSlot->GetPlayerCore().PrimaryColor);
}
