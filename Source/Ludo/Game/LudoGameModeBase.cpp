// Copyright Martin Furuberg. All Rights Reserved.

#include "LudoGameModeBase.h"

#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>

#include "LudoLog.h"
#include "Game/LudoGameInstance.h"
#include "Game/LudoGameState.h"
#include "Game/LudoPlayerController.h"
#include "Game/LudoAIController.h"
#include "Game/GamerState.h"
#include "Actors/Gamer.h"
#include "Actors/PlayerSlot.h"
#include "Actors/Board.h"
#include "Actors/Yard.h"
#include "Actors/Piece.h"
#include "UI/GameHUD.h"


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
}

void ALudoGameModeBase::InitGameState()
{
	Super::InitGameState();

	if (const TObjectPtr<ALudoGameState> LudoGameState = GetGameState<ALudoGameState>())
	{
		State = LudoGameState;
		GameEventsInterface = LudoGameState.Get();
		if (GameEventsInterface)
		{
			PlayStateChangedHandle = GameEventsInterface->GetPlayStateChangedDelegate().AddUObject(this, &ALudoGameModeBase::OnPlayStateChanged);
			PlayerReachedGoalHandle = GameEventsInterface->GetPlayerReachedGoalDelegate().AddUObject(this, &ALudoGameModeBase::OnPlayerReachedGoal);
		}

		// Initialize game settings from GI
		if (const TObjectPtr<ULudoGameInstance> GameInstance = GetGameInstance<ULudoGameInstance>())
		{
			const FGameSettings& Settings = GameInstance->GetTempGameSettings();
			bShouldSpawnCPU = Settings.NumPlayersCPU > 0;
			State->SetGameSettings(Settings);
			
			CreatePlayerSlots(Settings.NumPlayers);
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

bool ALudoGameModeBase::CheckGameReady() const
{
	// Check if all players are ready
	bool bReady = false;

	if (State)
	{
		// Game is ready if expected number of players is in
		bReady = State->GetNumPlayersReady() == State->GetSettings().NumPlayers;
	}

	return bReady;
}

bool ALudoGameModeBase::CheckGameFinished() const
{
	bool bFinished = false;

	if (State)
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

void ALudoGameModeBase::AddPlayerThrow(const FDieThrow& Throw) const
{
	if (!State) return;

	// add throw to dice throws list
	State->AddDieThrow(Throw);
}

void ALudoGameModeBase::PlayerPieceReachedGoal(const TObjectPtr<APiece> Piece) const
{
	if (!Piece || !State) return;

	const FPlayerCore& PlayerCore = Piece->GetPlayerCore();
	UE_LOG(LogLudoGM, Verbose, TEXT("%s (%s) reached goal"), *Piece->GetName(), *PlayerCore.DisplayName);

	Piece->SetInGoal(true);
	Piece->SetActorHiddenInGame(true);

	if (const TObjectPtr<APlayerSlot> PlayerSlot = State->GetPlayerSlot(PlayerCore))
	{
		const uint8 PlayerIndex = PlayerSlot->GetIndex();
		State->AddPlayerPieceInGoal(PlayerIndex);
	}
}

void ALudoGameModeBase::NextTurn()
{
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

void ALudoGameModeBase::EndGame() const
{
	if (const TObjectPtr<ALudoPlayerController> PlayerController = GetWorld()->GetFirstPlayerController<ALudoPlayerController>())
	{
		PlayerController->Server_ShowEndScreen();
	}
}

void ALudoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALudoGameModeBase::UpdateCurrentControllerState(const bool bIsStartingTurn /*= true*/) const
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

	if (const TObjectPtr<ALudoPlayerController> PC = Cast<ALudoPlayerController>(NewPlayer))
	{
		PC->SetGameEventsInterface(State.Get());

		// Check if PlayerController is server controlled
		if (bShouldSpawnCPU && NewPlayer->IsLocalPlayerController())
		{
			CreateCPUPlayers(State->GetSettings().NumPlayersCPU);
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
			Slot->SetPlayerCore(PlayerCores->Cores[Slot->GetIndex()]);
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

	if (TArray<APlayerSlot*> PlayerSlots = State->GetPlayerSlots(); !PlayerSlots.IsEmpty())
	{
		auto IsUnclaimed = [](const APlayerSlot* PlayerSlot)
		{
			return !PlayerSlot->IsClaimed();
		};
		if (const auto UnclaimedSlot = PlayerSlots.FindByPredicate(IsUnclaimed))
		{
			Slot = *UnclaimedSlot;
			Slot->TryClaim(Player);
		}
	}

	Player->StartSpot = Slot;

	return Slot;
}

void ALudoGameModeBase::CreatePlayerSlots(const uint8 PlayerCount) const
{
	TArray<APlayerSlot*> PlayerSlots;
	for (uint8 i=0; i < PlayerCount; i++)
	{
		const FRotator StartRotation = FRotator(0, i * (360.0f / 4), 0); // 4 sides of board, PlayerCount can be used but is bugged

		const TObjectPtr<APlayerSlot> Slot = GetWorld()->SpawnActor<APlayerSlot>(FVector::ZeroVector, StartRotation);
		Slot->SetIndex(i);
		PlayerSlots.Add(Slot);
	}

	if (State)
	{
		State->SetPlayerSlots(PlayerSlots);
	}
}

void ALudoGameModeBase::CreateCPUPlayers(const uint8 NumCPUPlayers)
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
	if (!GetBoard() || !State) return;

	// Spawn player pieces
	for (auto& PlayerState : State->PlayerArray)
	{
		const TObjectPtr<AGamerState> GamerState = Cast<AGamerState>(PlayerState);
		SpawnPiecesForPlayer(GamerState);
	}

	StartGame();
}

void ALudoGameModeBase::SpawnPiecesForPlayer(const TObjectPtr<AGamerState> GamerState) const
{
	if (!GetBoard()) return;

	const uint8 PlayerIndex = GamerState->GetPlayerIndex();
	const TObjectPtr<AGamer> Gamer = GamerState->GetPawn<AGamer>();

	UE_LOG(LogLudoGM, Verbose, TEXT("Spawn Pieces for player index %d"), PlayerIndex);
	if (const TObjectPtr<AYard> Yard = GetBoard()->GetYard(PlayerIndex))
	{
		Yard->SetGamer(Gamer);
		Yard->SpawnPieces();
	}
}

TArray<uint8> ALudoGameModeBase::GetEntryRolls(const EEntryRollMask EntryRollMask) const
{
	TArray<uint8> Numbers;
	switch (EntryRollMask)
	{
	case EEntryRollMask::ER_Low:
		Numbers.Append(EntryRollsLow);
		break;
	case EEntryRollMask::ER_High:
		Numbers.Append(EntryRollsHigh);
		break;
	default:
	case EEntryRollMask::ER_All:
		Numbers.Append(EntryRollsLow);
		Numbers.Append(EntryRollsHigh);
		break;
	}
	return Numbers;
}

int32 ALudoGameModeBase::GetNumPlayersTotal() const
{
	int32 PlayerCount = 0;
	for (FConstControllerIterator Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		if (const AController* OneController = Iterator->Get(); OneController->PlayerState)
		{
			PlayerCount++;
		}
	}
	return PlayerCount;
}

void ALudoGameModeBase::OnPlayStateChanged(AGamerState* GamerState, const EPlayState PlayState)
{
	if (PlayState != EPlayState::Ready || !CheckGameReady()) return;

	if (!GetBoard())
	{
		UE_LOG(LogLudoGM, Error, TEXT("Board not loaded, can't spawn player Yards!"));
		return;
	}

	// Check if BeginPlay has found all placed Yards 
	if (GetBoard()->bYardsFound)
	{
		SetupBoard();
	}
	else
	{
		// Wait for delegate exec
		TheBoard->OnFoundYards.BindLambda([this]()
		{
			SetupBoard();
		});
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
