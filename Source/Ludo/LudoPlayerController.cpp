// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
#include "Game/LudoGameState.h"
#include "Game/GamerState.h"
#include "Actors/Board.h"


ALudoPlayerController::ALudoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	ClickEventKeys.Add(EKeys::RightMouseButton);
	//bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	PlayerEventComponent = CreateDefaultSubobject<ULudoPlayerEventComponent>("PlayerEventComponent");
}

int8 ALudoPlayerController::GetPlayerIndex() const
{
	return GetPlayerState<AGamerState>()->GetPlayerIndex();
}

void ALudoPlayerController::Client_StartTurn_Implementation()
{
	bInTurn = true;
	UE_LOG(LogLudo, Verbose, TEXT("Client_StartTurn (%s)"), HasAuthority() ? TEXT("Auth") : TEXT("Client"));

	GetEvents()->OnPlayerTurn.Broadcast(true);
}

void ALudoPlayerController::Client_EndTurn_Implementation()
{
	bInTurn = false;
	UE_LOG(LogLudo, Verbose, TEXT("Client_EndTurn (%s)"), HasAuthority() ? TEXT("Auth") : TEXT("Client"));

	GetEvents()->OnPlayerTurn.Broadcast(false);
}

void ALudoPlayerController::Server_RequestEndTurn_Implementation()
{
	//TODO: Check if this player is actually in turn

	if (ALudoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>())
	{
		GameMode->NextTurn();
	}
}

bool ALudoPlayerController::Server_RequestEndTurn_Validate()
{
	return true;
}

void ALudoPlayerController::Server_NotifyOnReady_Implementation(APlayerState* PlayerStateReady)
{
	AGamerState* StateTyped = CastChecked<AGamerState>(PlayerStateReady);

	StateTyped->SetPlayState(EPlayState::Ready);

	if (ALudoGameState* GameState = GetWorld()->GetGameState<ALudoGameState>())
	{
		GameState->GetEvents()->OnPlayStateChange.Broadcast(StateTyped, EPlayState::Ready);
	}
}

class AGamerState* ALudoPlayerController::GetGamerState()
{
	return GetPlayerState<AGamerState>();
}

void ALudoPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (bClientReadyOnPlayerState)
	{
		Server_NotifyOnReady(GetPlayerState<AGamerState>());
	}
}

void ALudoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABoard::StaticClass());
	if (Actor)
	{
		TheBoard = Cast<ABoard>(Actor);
	}
}
