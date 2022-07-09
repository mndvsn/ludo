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

void ULudoGameInstance::CreateGameCPU(uint8 InPlayers = 4, uint8 InCPU = 3)
{
	if (UEngine* Engine = GetEngine())
	{
		Engine->AddOnScreenDebugMessage(0, 4, FColor::Red, FString::Printf(TEXT("Creating game with %d players, %d CPU"), InPlayers, InCPU));

		CreateGame(InPlayers, InCPU);
	}
}

void ULudoGameInstance::CreateGame(uint8 InPlayers, uint8 InCPU = 0)
{
	if (UWorld* World = GetWorld())
	{
		FString TravelURL;
		TravelURL = TEXT("/Game/Maps/L_Standard");
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