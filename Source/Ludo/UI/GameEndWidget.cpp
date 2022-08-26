// Copyright Martin Furuberg. All Rights Reserved.


#include "UI/GameEndWidget.h"

#include "LudoLog.h"
#include "Actors/Gamer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/GamerState.h"
#include "Actors/PlayerSlot.h"
#include "Game/LudoPlayerController.h"


void UGameEndWidget::SetValues(APlayerSlot* WinnerSlot)
{
	if (!WinnerSlot) return;

	const FString PlayerName = WinnerSlot->GetGamerState()->GetPlayerName();

	// Get PlayerSlot from controlled Gamer
	const TObjectPtr<ALudoPlayerController> PC = GetOwningPlayer<ALudoPlayerController>();
	if (const APlayerSlot* PlayerSlot = PC->GetGamer()->GetPlayerSlot())
	{
		// Set result of game
		const FString PlayerResult = (WinnerSlot->PlayerCore == PlayerSlot->PlayerCore) ? TEXT("Winner!") : TEXT("Lost!");
		PlayerWinLoseLabel->SetText(FText::FromString(PlayerResult));
	}
	
	WinnerLabel->SetText(FText::FromString(PlayerName));
	WinnerLabel->SetColorAndOpacity(WinnerSlot->PlayerCore.PrimaryColor);
}
