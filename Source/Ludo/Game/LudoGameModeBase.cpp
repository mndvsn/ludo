// Copyright Martin Furuberg. All Rights Reserved.

#include "LudoGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "LudoLog.h"
#include "LudoGameState.h"
#include "LudoPlayerController.h"
#include "LudoAIController.h"
#include "UI/GameHUD.h"
#include "GamerState.h"
#include "Actors/Gamer.h"
#include "Actors/LudoPlayerStart.h"



ALudoGameModeBase::ALudoGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameElements/BP_Gamer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = ALudoGameState::StaticClass();
	PlayerControllerClass = ALudoPlayerController::StaticClass();
	PlayerStateClass = AGamerState::StaticClass();
	HUDClass = AGameHUD::StaticClass();

	//bUseSeamlessTravel = true;

	PrimaryActorTick.bCanEverTick = false;
}

void ALudoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Override settings from URL
	NumPlayers = UGameplayStatics::GetIntOption(Options, TEXT("Players"), NumPlayers);
	NumPlayersCPU = UGameplayStatics::GetIntOption(Options, TEXT("CPU"), NumPlayersCPU);
	bShouldSpawnCPU = NumPlayersCPU > 0;

	CreatePlayerStarts(NumPlayers);
}

void ALudoGameModeBase::InitGameState()
{
	Super::InitGameState();

	if (ALudoGameState* State = GetGameState<ALudoGameState>())
	{
		State->SetPlayerCountForGame(NumPlayers);

		GameEventsInterface = State;
		if (GameEventsInterface)
		{
			PlayStateChangedHandle = GameEventsInterface->GetPlayStateChangedDelegate().AddUObject(this, &ALudoGameModeBase::OnPlayStateChanged);
		}
	}
}

void ALudoGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("PostLogin: %s"), *NewPlayer->GetName());
	
	Super::PostLogin(NewPlayer);
}

void ALudoGameModeBase::OnPostLogin(AController* NewPlayer)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("OnPostLogin: %s"), *NewPlayer->GetName());
}

void ALudoGameModeBase::GenericPlayerInitialization(AController* C)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("GenericPlayerInitialization: %s"), *C->GetName());
	
	Super::GenericPlayerInitialization(C);
}

bool ALudoGameModeBase::CheckGameReady()
{
	// Check if all players are ready
	bool Ready = false;

	//const int& NumCurrentPlayers = GetNumPlayersTotal();
	
	if (ALudoGameState* State = GetGameState<ALudoGameState>())
	{
		// Game is ready if expected number of players is in
		Ready = (State->GetNumPlayersReady() == NumPlayers);
	}

	return Ready;
}

void ALudoGameModeBase::StartGame()
{
	UE_LOG(LogLudoGM, Verbose, TEXT("StartGame"));

	// Go to first turn
	NextTurn();
}

void ALudoGameModeBase::NextTurn()
{
	ALudoGameState* State = GetGameState<ALudoGameState>();

	// If a players turn is ending
	if (State->GetCurrentPlayerIndex() >= 0)
	{
		UpdateCurrentControllerState(false);
	}
	
	// Set next turn
	State->AdvanceTurn();

	// Update controller in turn
	PlayerInTurn = CastChecked<ILudoGamerInterface>(State->GetGamerStateInTurn()->GetOwningController());

	UpdateCurrentControllerState(true);
}

void ALudoGameModeBase::UpdateCurrentControllerState(bool bIsStartingTurn /*= true*/)
{
	if (!ensure(PlayerInTurn)) return;

	AGamerState* GamerState = PlayerInTurn->GetGamerState();

	// Update their status 
	GamerState->SetPlayState(bIsStartingTurn ? EPlayState::Playing : EPlayState::Waiting);

	// Tell player turn has started or ended
	if (bIsStartingTurn)
	{
		UE_LOG(LogLudoGM, Verbose, TEXT("Start turn: P%d"), GamerState->GetPlayerIndex() + 1);
		PlayerInTurn->Client_StartTurn();
	}
	else
	{
		UE_LOG(LogLudoGM, Verbose, TEXT("End turn: P%d"), GamerState->GetPlayerIndex() + 1);
		PlayerInTurn->Client_EndTurn();
	}
}

void ALudoGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("HandleStartingNewPlayer: %s"), *NewPlayer->GetName());
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	SetupPlayer(NewPlayer);

	if (TObjectPtr<ALudoPlayerController> PC = Cast<ALudoPlayerController>(NewPlayer))
	{
		PC->SetGameEventsInterface(GetWorld()->GetGameState<ALudoGameState>());

		// Check if PlayerController is server controlled
		if (bShouldSpawnCPU && NewPlayer->IsLocalPlayerController())
		{
			CreateCPUPlayers(NumPlayersCPU);
		}
	}
}

void ALudoGameModeBase::SetupPlayer(AController* Player)
{
	const int& NumCurrentPlayers = GetNumPlayersTotal();

	AGamerState* PlayerState = Player->GetPlayerState<AGamerState>();
	ALudoPlayerStart* StartSpot = Cast<ALudoPlayerStart>(Player->StartSpot.Get());
	if (StartSpot)
	{
		PlayerState->SetPlayerIndex(StartSpot->GetPlayerSlot());
		ChangeName(Player, FString::Printf(TEXT("Player %d"), StartSpot->GetPlayerSlot()+1), false);
	}
	else
	{
		PlayerState->SetPlayerIndex(NumCurrentPlayers-1);
		ChangeName(Player, FString::Printf(TEXT("Player %d"), NumCurrentPlayers), false);
	}

	if (AGamer* Gamer = Player->GetPawn<AGamer>())
	{
		Gamer->UpdatePlayerLabel();
	}

	// Spawn Yard
}

AActor* ALudoGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("ChoosePlayerStart: %s"), *Player->GetName());
	ALudoPlayerStart* StartSpot = nullptr;

	if (!PlayerStarts.IsEmpty())
	{
		auto UnclaimedPlayerStart = PlayerStarts.FindByPredicate([](ALudoPlayerStart* PlayerStart) {
			return !PlayerStart->IsClaimed();
		});
		if (UnclaimedPlayerStart)
		{
			StartSpot = *UnclaimedPlayerStart;
			StartSpot->TryClaim(Player);
		}
	}

	Player->StartSpot = StartSpot;

	return StartSpot;
}

void ALudoGameModeBase::CreatePlayerStarts(uint8 PlayerCount)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	for (uint8 i=0; i < PlayerCount; i++)
	{
		const FRotator StartRotation = FRotator(0, i * (360.0f / 4), 0); // 4 sides of board, PlayerCount can be used but is bugged

		ALudoPlayerStart* PlayerStart = World->SpawnActor<ALudoPlayerStart>(FVector::ZeroVector, StartRotation);
		PlayerStart->SetPlayerSlot(i);
		PlayerStarts.Add(PlayerStart);
	}
}

void ALudoGameModeBase::CreateCPUPlayers(uint8 NumCPUPlayers)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("Create %d CPUs"), NumCPUPlayers);

	for (uint8 n = 0; n < NumCPUPlayers; ++n)
	{
		SpawnCPU();
	}

	bShouldSpawnCPU = false;
}

void ALudoGameModeBase::SpawnCPU()
{
	FActorSpawnParameters SpawnInfo;

	ALudoAIController* NewController = GetWorld()->SpawnActor<ALudoAIController>(ALudoAIController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

	if (NewController)
	{	
		if (NewController->PlayerState != nullptr)
		{
			NewController->PlayerState->SetPlayerName(TEXT("CPU"));
			NewController->PlayerState->SetIsABot(true);
		}

		CPUPlayers.Add(NewController);

		DispatchPostLogin(NewController);
		RestartPlayer(NewController);
		SetupPlayer(NewController);
		
		if (ALudoPlayerController* PC = GetWorld()->GetFirstPlayerController<ALudoPlayerController>())
		{
			PC->Server_NotifyOnReady(NewController->PlayerState);
		}
	}
}

int32 ALudoGameModeBase::GetNumPlayersTotal()
{
	/*int32 PlayerCount = GetNumPlayers();
	int32 CPUPlayerCount = CPUPlayers.Num();

	return PlayerCount + CPUPlayerCount;*/

	int32 PlayerCount = 0;
	for (FConstControllerIterator Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		AController* ControllerActor = Iterator->Get();
		if (ControllerActor && ControllerActor->PlayerState)
		{
			PlayerCount++;
		}
	}
	return PlayerCount;
}

void ALudoGameModeBase::OnPlayStateChanged(AGamerState* GamerState, EPlayState State)
{
	if (CheckGameReady())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game is ready!"));
		StartGame();
	}
}

void ALudoGameModeBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (GameEventsInterface && PlayStateChangedHandle.IsValid())
	{
		GameEventsInterface->GetPlayStateChangedDelegate().Remove(PlayStateChangedHandle);

		PlayStateChangedHandle.Reset();
		GameEventsInterface = nullptr;
	}
}
