// Copyright Martin Furuberg. All Rights Reserved.


#include "LudoPlayerController.h"

ALudoPlayerController::ALudoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	ClickEventKeys.Add(EKeys::RightMouseButton);
	//bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ALudoPlayerController::SetPlayerIndex(int val)
{
	PlayerIndex = val;
	UE_LOG(LogTemp, Warning, TEXT("Tilldelad player index: %d"), val);
}
