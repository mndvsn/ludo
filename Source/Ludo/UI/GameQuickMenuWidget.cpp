// Copyright Martin Furuberg. All Rights Reserved.


#include "GameQuickMenuWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "LudoPlayerController.h"
#include "Game/LudoGameState.h"
#include "Game/GamerState.h"
#include "UI/GameHUD.h"


UGameQuickMenuWidget::UGameQuickMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UGameQuickMenuWidget::NativeOnInitialized()
{
	if (UWorld* World = GetWorld())
	{
		if (ALudoGameState* State = World->GetGameState<ALudoGameState>())
		{
			State->GetEvents()->OnTurnChange.AddDynamic(this, &UGameQuickMenuWidget::OnTurnChange);
		}
	}

	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		PC->GetEvents()->OnPlayerTurn.AddDynamic(this, &UGameQuickMenuWidget::OnPlayerTurn);
	}

	OnShowMenu.AddDynamic(this, &UGameQuickMenuWidget::ButtonMenuReleased);
}

void UGameQuickMenuWidget::RemoveFromParent()
{
	if (UWorld* World = GetWorld())
	{
		if (ALudoGameState* State = World->GetGameState<ALudoGameState>())
		{
			State->GetEvents()->OnTurnChange.RemoveDynamic(this, &UGameQuickMenuWidget::OnTurnChange);
		}
	}

	if (ALudoPlayerController* PC = GetOwningPlayer<ALudoPlayerController>())
	{
		PC->GetEvents()->OnPlayerTurn.RemoveDynamic(this, &UGameQuickMenuWidget::OnPlayerTurn);
	}

	Super::RemoveFromParent();
}

void UGameQuickMenuWidget::OnTurnChange(uint8 NewPlayerIndex)
{
	ALudoGameState* State = GetWorld()->GetGameState<ALudoGameState>();
	if (!IsValid(State)) return;

	const AGamerState* GamerState = State->GetGamerStateForIndex(NewPlayerIndex);
	if (!IsValid(GamerState)) return;

	FText NewText = FText::FromString(GamerState->GetPlayerName());
	LabelPlayerTurn->SetText(NewText);
}

void UGameQuickMenuWidget::ButtonMenuReleased()
{
	if (AGameHUD* GameHUD = GetOwningPlayer()->GetHUD<AGameHUD>())
	{
		GameHUD->ShowInGameMenu();
	}
}

void UGameQuickMenuWidget::OnPlayerTurn(bool IsPlayerTurn)
{
	if (IsPlayerTurn)
	{
		PlayerTurnBackground->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PlayerTurnBackground->SetVisibility(ESlateVisibility::Hidden);
	}
}
