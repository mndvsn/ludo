// Copyright Martin Furuberg. All Rights Reserved.


#include "GameQuickMenuWidget.h"

#include "Components/TextBlock.h"

#include "Game/LudoGameState.h"
#include "UI/GameHUD.h"


UGameQuickMenuWidget::UGameQuickMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Super::NativeOnInitialized();
}

void UGameQuickMenuWidget::NativeOnInitialized()
{
	if (GetWorld() == nullptr) return;

	ALudoGameState* State = GetWorld()->GetGameState<ALudoGameState>();
	if (State == nullptr) return;

	State->GetEvents()->OnTurnChange.AddDynamic(this, &UGameQuickMenuWidget::OnTurnChange);

	OnShowMenu.AddDynamic(this, &UGameQuickMenuWidget::ButtonMenuReleased);
}

void UGameQuickMenuWidget::RemoveFromParent()
{
	if (GetWorld() != nullptr)
	{
		ALudoGameState* State = GetWorld()->GetGameState<ALudoGameState>();
		if (State != nullptr)
		{
			State->GetEvents()->OnTurnChange.RemoveDynamic(this, &UGameQuickMenuWidget::OnTurnChange);
		}
	}

	Super::RemoveFromParent();
}

void UGameQuickMenuWidget::OnTurnChange(uint8 NewPlayerIndex)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("Player turn: %d"), NewPlayerIndex));
	LabelPlayerTurn->SetText(NewText);
}

void UGameQuickMenuWidget::ButtonMenuReleased()
{
	AGameHUD* GameHUD = GetOwningPlayer()->GetHUD<AGameHUD>();
	if (!ensure(IsValid(GameHUD))) return;

	GameHUD->ShowInGameMenu();
}
