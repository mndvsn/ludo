// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoGameInstance.h"


void ULudoGameInstance::Init()
{
	Super::Init();

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->OnNetworkFailure().AddUObject(this, &ULudoGameInstance::HandleNetworkFailure);
	Engine->OnTravelFailure().AddUObject(this, &ULudoGameInstance::HandleTravelFailure);
}

void ULudoGameInstance::CreateGame(const uint32 InRandomSeed, const uint8 InPlayers, const uint8 InCPU)
{
	if (UWorld* World = GetWorld())
	{
		FGameSettings Settings;
		Settings.Seed = InRandomSeed;
		Settings.NumPlayers = InPlayers;
		Settings.NumPlayersCPU = InCPU;

		StoreTempGameSettings(Settings);
		
		GetEngine()->AddOnScreenDebugMessage(0, 4, FColor::Red, FString::Printf(TEXT("Creating game with %d players (%d CPU) and seed %ld"), InPlayers, InCPU, InRandomSeed));

		const FString TravelURL = TEXT("/Game/Maps/L_Standard");
		World->ServerTravel(TravelURL);
	}
}

void ULudoGameInstance::EndGame()
{
	ReturnToMainMenu();
}

void ULudoGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Red, ErrorString);
}

void ULudoGameInstance::HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Red, ErrorString);
}

void ULudoGameInstance::Shutdown()
{
	Super::Shutdown();
}