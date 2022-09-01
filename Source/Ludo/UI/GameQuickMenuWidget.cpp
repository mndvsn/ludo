// Copyright Martin Furuberg. All Rights Reserved.


#include "GameQuickMenuWidget.h"

#include <Components/Image.h>

#include "LudoLog.h"
#include "Game/LudoPlayerController.h"
#include "Game/LudoGameState.h"
#include "Game/GamerState.h"
#include "UI/GameHUD.h"


void UGameQuickMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GameEventsInterface = GetWorld()->GetGameState<ALudoGameState>())
	{
		TurnChangedHandle = GameEventsInterface->GetTurnChangedDelegate().AddUObject(this, &UGameQuickMenuWidget::OnTurnChange);
	}

	if (PlayerEventsInterface = GetOwningPlayer<ALudoPlayerController>())
	{
		PlayerEventsInterface->GetPlayerTurnDelegate().AddDynamic(this, &UGameQuickMenuWidget::OnPlayerTurn);
	}

	OnShowMenu.AddDynamic(this, &UGameQuickMenuWidget::ButtonMenuReleased);
}

void UGameQuickMenuWidget::RemoveFromParent()
{
	if (GameEventsInterface && TurnChangedHandle.IsValid())
	{
		GameEventsInterface->GetTurnChangedDelegate().Remove(TurnChangedHandle);

		TurnChangedHandle.Reset();
		GameEventsInterface = nullptr;
	}

	if (PlayerEventsInterface)
	{
		PlayerEventsInterface->GetPlayerTurnDelegate().RemoveDynamic(this, &UGameQuickMenuWidget::OnPlayerTurn);

		PlayerEventsInterface = nullptr;
	}

	Super::RemoveFromParent();
}

void UGameQuickMenuWidget::OnTurnChange(uint8 NewPlayerIndex)
{
	UE_LOG(LogLudo, Verbose, TEXT("OnTurnChange: %d (%s)"), NewPlayerIndex, GetOwningPlayer()->HasAuthority() ? TEXT("Auth") : TEXT("Client"));
	if (ALudoGameState* State = Cast<ALudoGameState>(GameEventsInterface.GetObject()))
	{
		const TObjectPtr<AGamerState> GamerState = State->GetGamerStateForIndex(NewPlayerIndex);
		if (GamerState)
		{
			const FString PlayerName = GamerState->GetPlayerName();
			OnPlayerTurnNameChanged.Broadcast(PlayerName);
		}
	}
}

void UGameQuickMenuWidget::ButtonMenuReleased()
{
	if (AGameHUD* GameHUD = GetOwningPlayer()->GetHUD<AGameHUD>())
	{
		GameHUD->ShowInGameMenu();
	}
}

void UGameQuickMenuWidget::OnPlayerTurn_Implementation(bool bIsPlayerTurn)
{
	
}
