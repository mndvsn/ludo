// Copyright Martin Furuberg. All Rights Reserved.

#include "LudoGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "LudoGameState.h"
#include "LudoPlayerController.h"
#include "UI/GameHUD.h"
#include "GamerState.h"
#include "Actors/Gamer.h"
#include "Actors/LudoPlayerStart.h"

DEFINE_LOG_CATEGORY(LogLudoGM);


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

	PrimaryActorTick.bCanEverTick = false;

	// Set optimal number of players in this game mode
	MaxNumberOfPlayers = 4;
}

void ALudoGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("New player: %s"), *NewPlayer->GetName());

	const int &NumPlayers = GetNumPlayers();

	if (CheckGameReady())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game is ready!"));
		StartGame();
	}
}

bool ALudoGameModeBase::CheckGameReady()
{
	return GetNumPlayers() == 2; // or expected number of players
}

void ALudoGameModeBase::StartGame()
{
	ALudoGameState* State = GetGameState<ALudoGameState>();
	
	// Go to first turn
	State->AdvanceTurn();

	// Set controller in turn
	PlayerControllerInTurn = CastChecked<ALudoPlayerController>(State->GetGamerStateInTurn()->GetPlayerController());
	
	// Tell player controller in turn
	PlayerControllerInTurn->Client_StartTurn();

	UE_LOG(LogTemp, Warning, TEXT("-- Start: P%d --"), State->GetCurrentPlayerIndex() + 1);
}

void ALudoGameModeBase::NextTurn()
{
	ALudoGameState* State = GetGameState<ALudoGameState>();

	// Tell player turn has ended
	PlayerControllerInTurn->Client_EndTurn();

	UE_LOG(LogTemp, Warning, TEXT("-- End: P%d --"), State->GetCurrentPlayerIndex() + 1);

	State->AdvanceTurn();

	// Update controller in turn
	PlayerControllerInTurn = CastChecked<ALudoPlayerController>(State->GetGamerStateInTurn()->GetPlayerController());

	// Tell next controller in turn
	PlayerControllerInTurn->Client_StartTurn();

	UE_LOG(LogTemp, Warning, TEXT("-- Start: P%d --"), State->GetCurrentPlayerIndex() + 1);
}

void ALudoGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	const int &NumPlayers = GetNumPlayers();
	
	ALudoPlayerController* Player = CastChecked<ALudoPlayerController>(NewPlayer);
	if (!Player) return;

	ChangeName(Player, FString::Printf(TEXT("Player %d"), NumPlayers), false);
	
	AGamerState* PlayerState = Player->GetPlayerState<AGamerState>();
	PlayerState->SetPlayerIndex(NumPlayers - 1);

	//-----------
	// Set player label as server
	if (AGamer* Gamer = Player->GetPawn<AGamer>())
	{
		Gamer->UpdatePlayerLabel();
	}

	// Spawn Yard
}

AActor* ALudoGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Give player start -> %s"), *Player->GetName());
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

	return StartSpot;
}

void ALudoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("INIT!"));

	if (!MaxNumberOfPlayers) return;
	
	int NumberOfPlayers = MaxNumberOfPlayers;

	CreatePlayerStarts(NumberOfPlayers);
}

void ALudoGameModeBase::CreatePlayerStarts(uint8 PlayerCount)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	for (uint8 i=0; i < PlayerCount; i++)
	{
		const FRotator StartRotation = FRotator(0, i * (360.0f / PlayerCount), 0);

		ALudoPlayerStart* PlayerStart = World->SpawnActor<ALudoPlayerStart>(FVector::ZeroVector, StartRotation);
		PlayerStart->SetPlayerSlot(i);
		PlayerStarts.Add(PlayerStart);
	}
}