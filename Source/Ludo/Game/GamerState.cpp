// Copyright Martin Furuberg. All Rights Reserved.


#include "GamerState.h"
#include "Net/UnrealNetwork.h"

#include "LudoLog.h"
#include "LudoPlayerController.h"
#include "Common/PlayerCore.h"
#include "Actors/Gamer.h"

AGamerState::AGamerState()
{
	// Use custom player names
	// expects GetPlayerNameCustom()
	//bUseCustomPlayerNames = true;

	PlayState = EPlayState::Transitioning;
}

void AGamerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamerState, PlayerIndex);
	DOREPLIFETIME(AGamerState, PlayState);
	DOREPLIFETIME(AGamerState, PlayerCore);
}

/**
 * Player name updated
 */
void AGamerState::UpdatePlayerName()
{
	OnRep_PlayerName();
}

void AGamerState::OnRep_PlayerName()
{
	if (AController* OwnerController = GetOwner<AController>())
	{
		if (AGamer* Gamer = Cast<AGamer>(OwnerController->GetPawn()))
		{
			Gamer->UpdatePlayerLabel();
		}
	}
}

void AGamerState::OnRep_PlayerIndex()
{
	UE_LOG(LogLudoGS, Verbose, TEXT("OnRep_PlayerIndex: %d (Client)"), PlayerIndex);
	GetWorld()->GetFirstPlayerController<ALudoPlayerController>()->CheckPlayerStates();
}

void AGamerState::SetPlayerIndex(int NewIndex)
{
	UE_LOG(LogLudoGS, Verbose, TEXT("SetPlayerIndex: %d (Auth)"), NewIndex);
	PlayerIndex = NewIndex;
}

void AGamerState::SetPlayerCore(TObjectPtr<UPlayerCore> NewCore)
{
	UE_LOG(LogLudoGS, Verbose, TEXT("SetPlayerCore: %s > %d (Auth)"), *GetPlayerName(), *NewCore->DisplayName);
	PlayerCore = NewCore;
}

void AGamerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AGamerState* GamerState = CastChecked<AGamerState>(PlayerState))
	{
		GamerState->SetPlayerIndex(GetPlayerIndex());
		GamerState->SetPlayState(PlayState);
		GamerState->SetPlayerCore(PlayerCore);
	}
}

void AGamerState::OverrideWith(APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

	if (AGamerState* GamerState = CastChecked<AGamerState>(PlayerState))
	{
		SetPlayState(GamerState->PlayState);
		SetPlayerIndex(GamerState->PlayerIndex);
		SetPlayerCore(GamerState->PlayerCore);
	}
}
