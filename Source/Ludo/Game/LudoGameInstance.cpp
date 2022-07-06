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

void ULudoGameInstance::CreateGameCPU(uint32 NumCPUPlayers)
{
	if (GetEngine() == nullptr) return;
	GetEngine()->AddOnScreenDebugMessage(0, 4, FColor::Red, FString::Printf(TEXT("Creating game with CPU: %d"), NumCPUPlayers));

	CreateGame();
}

void ULudoGameInstance::CreateGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/L_Standard");
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