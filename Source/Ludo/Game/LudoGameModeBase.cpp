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

	CheckGameReady();
}

void ALudoGameModeBase::CheckGameReady()
{
	if (GetNumPlayers() == 2) // or expected number of players
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game is ready!"));

		// Go to first turn
		GetGameState<ALudoGameState>()->AdvanceTurn();
	}
}

void ALudoGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	const int &NumPlayers = GetNumPlayers();

	if (ALudoPlayerController* Player = CastChecked<ALudoPlayerController>(NewPlayer))
	{
		Player->SetPlayerIndex(NumPlayers - 1);
		Player->GetPlayerState<AGamerState>()->SetPlayerName(FString::Printf(TEXT("Player %d"), NumPlayers));
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (ALudoPlayerController* Player = CastChecked<ALudoPlayerController>(NewPlayer))
	{
		// Set player label as server
		if (AGamer* Gamer = Player->GetPawn<AGamer>())
		{
			Gamer->UpdatePlayerLabel();
		}

		// Spawn Yard

	}
}

AActor* ALudoGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Give player start -> %s"), *Player->GetName());
	ALudoPlayerStart* StartSpot = nullptr;

	if (AvailablePlayerStarts.Num() > 0)
	{
		StartSpot = AvailablePlayerStarts[0];
		AvailablePlayerStarts.RemoveAt(0);
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
		FRotator StartRotation;
		StartRotation.Yaw = i * (360.0f / PlayerCount);

		ALudoPlayerStart* PlayerStart = World->SpawnActor<ALudoPlayerStart>(FVector::ZeroVector, StartRotation);
		PlayerStart->SetPlayerIndex(i);
		AvailablePlayerStarts.Add(PlayerStart);
	}
}