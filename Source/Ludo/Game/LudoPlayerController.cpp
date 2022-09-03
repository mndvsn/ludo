// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoPlayerController.h"

#include <Kismet/GameplayStatics.h>

#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
#include "Game/LudoGameState.h"
#include "Game/GamerState.h"
#include "Actors/Gamer.h"
#include "Actors/Board.h"
#include "UI/GameHUD.h"


ALudoPlayerController::ALudoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	ClickEventKeys.Add(EKeys::RightMouseButton);
	//bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

int8 ALudoPlayerController::GetPlayerIndex() const
{
	return GetPlayerState<AGamerState>()->GetPlayerIndex();
}

void ALudoPlayerController::Client_StartTurn_Implementation()
{
	bInTurn = true;
	UE_LOG(LogLudo, Verbose, TEXT("Client_StartTurn (%s)"), HasAuthority() ? TEXT("Auth") : TEXT("Client"));

	OnPlayerTurn.Broadcast(true);
}

void ALudoPlayerController::Client_EndTurn_Implementation()
{
	bInTurn = false;
	UE_LOG(LogLudo, Verbose, TEXT("Client_EndTurn (%s)"), HasAuthority() ? TEXT("Auth") : TEXT("Client"));

	OnPlayerTurn.Broadcast(false);
}

void ALudoPlayerController::Server_RequestEndTurn_Implementation()
{
	if (ALudoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>())
	{
		GameMode->NextTurn();
	}
}

bool ALudoPlayerController::Server_RequestEndTurn_Validate()
{
	//TODO: Check if this player is actually in turn
	return true;
}

void ALudoPlayerController::ThrowDie()
{
	//TODO: Check if this player is actually in turn

	const TObjectPtr<AGamer> Gamer = GetGamer();
	Gamer->Server_ThrowDie();
}

void ALudoPlayerController::Server_NotifyOnReady_Implementation(APlayerState* PlayerStateReady)
{
	UE_LOG(LogLudo, Verbose, TEXT("Server_NotifyOnReady: %s"), *GetName());

	const TObjectPtr<AGamerState> StateTyped = Cast<AGamerState>(PlayerStateReady);
	StateTyped->SetPlayState(EPlayState::Ready);

	if (GameEventsInterface)
	{
		GameEventsInterface->GetPlayStateChangedDelegate().Broadcast(StateTyped, EPlayState::Ready);
	}
}

void ALudoPlayerController::Server_ShowEndScreen_Implementation()
{
	if (const TObjectPtr<ALudoGameState> State = GetWorld()->GetGameState<ALudoGameState>())
	{
		const TObjectPtr<APlayerSlot> WinnerSlot = State->GetPlayerSlot(State->GetWinner());
		GetGamer()->Client_ShowEndScreen(WinnerSlot.Get());
	}
}

void ALudoPlayerController::CheckPlayerStates()
{
	if (!PlayerState) return; // PlayerState not replicated yet, wait

	if (const ALudoGameState* GameState = GetWorld()->GetGameState<ALudoGameState>())
	{
		// Check if we have received an updated (+PlayerIndex) PlayerState for all (expected) other clients
		if (GameState->GetNumPlayersReplicated() == GameState->GetPlayerCountForGame())
		{
			Server_NotifyOnReady(PlayerState);
		}
	}
}

TObjectPtr<AGamerState> ALudoPlayerController::GetGamerState()
{
	return GetPlayerState<AGamerState>();
}

void ALudoPlayerController::OnRep_PlayerState()
{
	UE_LOG(LogLudo, Verbose, TEXT("OnRep_PlayerState: %s (%s)"), *GetName(), (HasAuthority() ? TEXT("Auth") : TEXT("Client")));
	Super::OnRep_PlayerState();

	if (!GameEventsInterface)
	{
		GameEventsInterface = GetWorld()->GetGameState<ALudoGameState>();
	}

	// Don't proceed if we are beyond loading/transition
	if (Cast<AGamerState>(PlayerState)->GetPlayState() > EPlayState::Transitioning) return;

	CheckPlayerStates();
}

void ALudoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (const TObjectPtr<AActor> Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABoard::StaticClass()))
	{
		GameBoard = Cast<ABoard>(Actor);

		if (HasAuthority())
		{
			const TObjectPtr<ALudoGameModeBase> GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>();
			GameMode->SetBoard(GameBoard);
		}
	}
}

void ALudoPlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	Super::ClientSetHUD_Implementation(NewHUDClass);

	if (!HasAuthority()) return;

	// UI initialized callback for server controller
	if (const TObjectPtr<AGameHUD> PlayerHUD = GetHUD<AGameHUD>())
	{
		PlayerHUD->OnGameHUDReady.BindLambda([this]()
		{
			Server_NotifyOnReady(GetGamerState());
		});
	}
}

void ALudoPlayerController::OnGameHUDReady()
{
	//TODO: Implement?
}

TObjectPtr<AGamer> ALudoPlayerController::GetGamer()
{
	return GetPawn<AGamer>();
}
