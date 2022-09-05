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

void ULudoGameInstance::CreateGameCPU(const uint32 InRandomSeed, const uint8 InPlayers, const uint8 InCPU) const
{
	if (UEngine* Engine = GetEngine())
	{
		Engine->AddOnScreenDebugMessage(0, 4, FColor::Red, FString::Printf(TEXT("Creating game with %d players, %d CPU"), InPlayers, InCPU));

		CreateGame(InRandomSeed, InPlayers, InCPU);
	}
}

void ULudoGameInstance::CreateGame(const uint32 InRandomSeed, const uint8 InPlayers, const uint8 InCPU) const
{
	if (UWorld* World = GetWorld())
	{
		FString TravelURL = TEXT("/Game/Maps/L_Standard");
		TravelURL += FString::Printf(TEXT("?Seed=%ld"), InRandomSeed);
		TravelURL += FString::Printf(TEXT("?Players=%d"), InPlayers);
		TravelURL += FString::Printf(TEXT("?CPU=%d"), InCPU);

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