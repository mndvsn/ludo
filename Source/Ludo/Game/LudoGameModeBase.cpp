// Copyright Martin Furuberg. All Rights Reserved.

#include "LudoGameModeBase.h"
#include "GamerState.h"
#include "../LudoPlayerController.h"
#include "../Actors/Gamer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"


ALudoGameModeBase::ALudoGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameElements/BP_Gamer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ALudoPlayerController::StaticClass();
	PlayerStateClass = AGamerState::StaticClass();

	PrimaryActorTick.bCanEverTick = false;

	// Set optimal number of players in this game mode
	MaxNumberOfPlayers = 4;
}

void ALudoGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("New player: %s"), *NewPlayer->GetName());

	const int &NumPlayers = GetNumPlayers();

	if (NumPlayers == 2) // or expected number of players
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game is ready!"));
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
	APlayerStart* StartSpot = nullptr;

	if (AvailableStartPoints.Num() > 0)
	{
		StartSpot = AvailableStartPoints[0];
		AvailableStartPoints.RemoveAt(0);
	}

	return StartSpot;
}

//bool ALudoGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
//{
//	return true;
//}

void ALudoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("INIT!"));

	if (!MaxNumberOfPlayers) return;
	
	int NumberOfPlayers = MaxNumberOfPlayers;

	// Find all PlayerStarts
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		AvailableStartPoints.Add(*It);
	}
	AvailableStartPoints.Sort([](const APlayerStart& A, const APlayerStart& B)
	{
		int StartIndexA = FCString::Atoi(*A.PlayerStartTag.ToString());
		int StartIndexB = FCString::Atoi(*B.PlayerStartTag.ToString());
		return StartIndexA < StartIndexB;
	});
	UE_LOG(LogTemp, Warning, TEXT("StartPoints: %d"), AvailableStartPoints.Num());

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AvailableStartPoints);
/*
	for (int i = 0; i < NumberOfPlayers; i++)
	{
		UWorld* World = GetWorld();

		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		
		FRotator PlayerRotation = FRotator(0.f, 0.f, 90.f * i);

		const AGamer* Player = World->SpawnActor<AGamer>(AGamer::StaticClass(), FVector::ZeroVector, PlayerRotation, SpawnParams);

		Players->Add(Player);

		UE_LOG(LogTemp, Warning, TEXT("Player: %s"), Player);
	}

	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->Possess(Players[0]);*/
}
