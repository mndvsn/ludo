// Copyright Martin Furuberg. All Rights Reserved.

#include "LudoGameModeBase.h"

#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>

#include "LudoLog.h"
#include "LudoGameState.h"
#include "LudoPlayerController.h"
#include "LudoAIController.h"
#include "UI/GameHUD.h"
#include "GamerState.h"
#include "Actors/Gamer.h"
#include "Actors/PlayerSlot.h"
#include "Actors/Board.h"
#include "Actors/Yard.h"
#include "Actors/Piece.h"


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
			PlayerReachedGoalHandle = GameEventsInterface->GetPlayerReachedGoalDelegate().AddUObject(this, &ALudoGameModeBase::OnPlayerReachedGoal);
		}
	}

	CreatePlayerSlots(NumPlayers);
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

bool ALudoGameModeBase::CheckGameReady() const
{
	// Check if all players are ready
	bool bReady = false;	
	
	if (ALudoGameState* State = GetGameState<ALudoGameState>())
	{
		// Game is ready if expected number of players is in
		bReady = (State->GetNumPlayersReady() == State->GetPlayerCountForGame());
	}

	return bReady;
}

bool ALudoGameModeBase::CheckGameFinished() const
{
	bool bFinished = false;
	
	if (ALudoGameState* State = GetGameState<ALudoGameState>())
	{
		bFinished = State->HasGameEnded();
	}

	return bFinished;
}

void ALudoGameModeBase::StartGame()
{
	UE_LOG(LogLudoGM, Verbose, TEXT("StartGame"));

	// Go to first turn
	NextTurn();
}

void ALudoGameModeBase::AddPlayerThrow(FDieThrow Throw)
{
	// add throw to dice throws list
	ALudoGameState* State = GetGameState<ALudoGameState>();
	State->AddDieThrow(Throw);
}

void ALudoGameModeBase::PlayerPieceReachedGoal(const TObjectPtr<APiece> Piece)
{
	FPlayerCore PlayerCore = Piece->GetPlayerCore();
	UE_LOG(LogLudoGM, Verbose, TEXT("%s (%s) reached goal"), *Piece->GetName(), *PlayerCore.DisplayName);

	Piece->SetInGoal(true);
	Piece->SetActorHiddenInGame(true);

	ALudoGameState* State = GetGameState<ALudoGameState>();
	uint8 PlayerIndex = State->GetPlayerSlot(PlayerCore)->GetIndex();
	State->AddPlayerPieceInGoal(PlayerIndex);
}

void ALudoGameModeBase::NextTurn()
{
	ALudoGameState* State = GetGameState<ALudoGameState>();

	// If a players turn is ending
	if (State->GetCurrentPlayerIndex() >= 0)
	{
		UpdateCurrentControllerState(false);
	}
	
	if (CheckGameFinished())
	{
		return EndGame();
	}

	// Set next turn
	State->AdvanceTurn();

	// Update controller in turn
	PlayerInTurn = CastChecked<ILudoControllerInterface>(State->GetGamerStateInTurn()->GetOwningController());

	UpdateCurrentControllerState(true);
}

void ALudoGameModeBase::EndGame()
{
	if (TObjectPtr<ALudoPlayerController> PlayerController = GetWorld()->GetFirstPlayerController<ALudoPlayerController>())
	{
		PlayerController->Server_ShowEndScreen();
	}
}

void ALudoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
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

void ALudoGameModeBase::OnPlayerReachedGoal(const uint8 Player, const uint8 InGoalTotal)
{
	if (InGoalTotal == 4)
	{
		UE_LOG(LogLudoGM, Display, TEXT("Player index %d won!"), Player);
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
	TObjectPtr<APlayerSlot> Slot = Cast<APlayerSlot>(Player->StartSpot);
	if (Slot)
	{
		PlayerState->SetPlayerIndex(Slot->GetIndex());
		ChangeName(Player, FString::Printf(TEXT("Player %d"), Slot->GetIndex()+1), false);
		
		Slot->SetPlayerOrder(Slot->GetIndex());

		if (PlayerCores && !PlayerCores->Cores.IsEmpty())
		{
			// Set PlayerCore to index of array for now
			Slot->PlayerCore = PlayerCores->Cores[Slot->GetIndex()];
		}
	}
	else
	{
		PlayerState->SetPlayerIndex(NumCurrentPlayers-1);
		ChangeName(Player, FString::Printf(TEXT("Player %d"), NumCurrentPlayers), false);
	}

	if (TObjectPtr<AGamer> Gamer = Player->GetPawn<AGamer>())
	{
		Gamer->UpdatePlayerLabel();

		if (Slot)
		{
			Slot->SetGamer(Gamer);
			Gamer->SetPlayerSlot(Slot);
		}
	}
}

AActor* ALudoGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogLudoGM, Verbose, TEXT("ChoosePlayerStart: %s"), *Player->GetName());
	APlayerSlot* Slot = nullptr;

	ALudoGameState* LudoGameState = Cast<ALudoGameState>(GameState);
	TArray<APlayerSlot*> PlayerSlots = LudoGameState->GetPlayerSlots();

	if (!PlayerSlots.IsEmpty())
	{
		auto UnclaimedSlot = PlayerSlots.FindByPredicate([](APlayerSlot* PlayerSlot) {
			return !PlayerSlot->IsClaimed();
		});
		if (UnclaimedSlot)
		{
			Slot = *UnclaimedSlot;
			Slot->TryClaim(Player);
		}
	}

	Player->StartSpot = Slot;

	return Slot;
}

void ALudoGameModeBase::CreatePlayerSlots(uint8 PlayerCount)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	TArray<APlayerSlot*> PlayerSlots;
	for (uint8 i=0; i < PlayerCount; i++)
	{
		const FRotator StartRotation = FRotator(0, i * (360.0f / 4), 0); // 4 sides of board, PlayerCount can be used but is bugged

		APlayerSlot* Slot = World->SpawnActor<APlayerSlot>(FVector::ZeroVector, StartRotation);
		Slot->SetIndex(i);
		PlayerSlots.Add(Slot);
	}

	if (TObjectPtr<ALudoGameState> LudoGameState = Cast<ALudoGameState>(GameState))
	{
		LudoGameState->SetPlayerSlots(PlayerSlots);
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

void ALudoGameModeBase::SetBoard(TObjectPtr<ABoard> BoardActor)
{
	if (!BoardActor) return;
	TheBoard = BoardActor;
}

void ALudoGameModeBase::SetupBoard()
{
	if (!GetBoard()) return;

	// Spawn player pieces
	for (auto& PlayerState : GetGameState<ALudoGameState>()->PlayerArray)
	{
		TObjectPtr<AGamerState> GamerState = Cast<AGamerState>(PlayerState);
		SpawnPiecesForPlayer(GamerState);
	}

	StartGame();
}

void ALudoGameModeBase::SpawnPiecesForPlayer(TObjectPtr<AGamerState> GamerState)
{
	if (!GetBoard()) return;

	const uint8 PlayerIndex = GamerState->GetPlayerIndex();
	const TObjectPtr<AGamer> Gamer = Cast<AGamer>(GamerState->GetPawn());

	UE_LOG(LogLudoGM, Verbose, TEXT("Spawn Pieces for player index %d"), PlayerIndex);
	if (TObjectPtr<AYard> Yard = GetBoard()->GetYard(PlayerIndex))
	{	
		Yard->SetGamer(Gamer);
		Yard->SpawnPieces();
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
	if (State != EPlayState::Ready) return;

	if (!CheckGameReady()) return;
	
	if (GetBoard())
	{
		if (GetBoard()->bYardsFound)
		{
			SetupBoard();				
		}
		else
		{
			TheBoard->OnFoundYards.BindLambda([this]()
			{
				SetupBoard();
			});
		}
	}
	else
	{
		UE_LOG(LogLudoGM, Error, TEXT("Board not loaded, can't spawn player Yards!"));
	}
}

void ALudoGameModeBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (GameEventsInterface && PlayStateChangedHandle.IsValid())
	{
		GameEventsInterface->GetPlayStateChangedDelegate().Remove(PlayStateChangedHandle);
		PlayStateChangedHandle.Reset();

		GameEventsInterface->GetPlayerReachedGoalDelegate().Remove(PlayerReachedGoalHandle);
		PlayerReachedGoalHandle.Reset();

		GameEventsInterface = nullptr;
	}
}
