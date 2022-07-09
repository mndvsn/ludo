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
	check(State);

	const AGamerState* GamerState = State->GetGamerStateForIndex(NewPlayerIndex);
	check(GamerState);

	FString PlayerName = GamerState->GetPlayerName();

	UE_LOG(LogTemp, Warning, TEXT("Player name: %s"), *PlayerName);
	OnPlayerTurnNameChanged.Broadcast(PlayerName);
}

void UGameQuickMenuWidget::ButtonMenuReleased()
{
	if (AGameHUD* GameHUD = GetOwningPlayer()->GetHUD<AGameHUD>())
	{
		GameHUD->ShowInGameMenu();
	}
}

void UGameQuickMenuWidget::OnPlayerTurn_Implementation(bool IsPlayerTurn)
{
	
}
