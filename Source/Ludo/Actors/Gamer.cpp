// Copyright Martin Furuberg. All Rights Reserved.


#include "Gamer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "LudoLog.h"
#include "Game/LudoGameInstance.h"
#include "Game/LudoGameModeBase.h"
#include "Game/LudoGameState.h"
#include "Game/LudoPlayerController.h"
#include "Game/GamerState.h"
#include "UI/GameHUD.h"

// Sets default values
AGamer::AGamer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CameraArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArmComp->SetupAttachment(RootComponent);
	CameraArmComp->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	CameraArmComp->bDoCollisionTest = false;
	CameraArmComp->bUsePawnControlRotation = false;

	CameraArmComp->TargetArmLength = CameraZoomDefault;
	CameraArmComp->bEnableCameraLag = true;
	CameraArmComp->CameraLagSpeed = 1.0f;
	CameraArmComp->bEnableCameraRotationLag = true;
	CameraArmComp->CameraRotationLagSpeed = 9.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(CameraArmComp, USpringArmComponent::SocketName);
	CameraComp->SetFieldOfView(30.0f);
	CameraComp->bUsePawnControlRotation = false;
	
	bZoomToggled = false;
	CameraZoomCurrent = CameraZoomTarget = CameraZoomDefault;
	
	PlayerLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerLabel"));
	PlayerLabel->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGamer::BeginPlay()
{
	Super::BeginPlay();

	CameraRotationStep = (360.0f / 4);// / PC->InputYawScale; // 90

	if (TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>())
	{
		GameState->GetDieThrowDelegate().AddUObject(this, &AGamer::OnDieThrow);
	}
}

void AGamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		// Smooth zoom, interpolate current to target
		if (CameraZoomCurrent != CameraZoomTarget)
		{
			CameraZoomCurrent = FMath::FInterpTo(CameraZoomCurrent, CameraZoomTarget, DeltaTime, 4.0f);
			CameraArmComp->TargetArmLength = CameraZoomCurrent;
		}

		FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraComp->GetComponentLocation(), FVector::ZeroVector);
		CameraComp->SetWorldRotation(CameraRotation);
	}
}

// Called to bind functionality to input
void AGamer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// System
	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &AGamer::MenuKeyPressed);

	// Camera
	PlayerInputComponent->BindAxis("CameraUp", this, &AGamer::PushCameraUp);
	PlayerInputComponent->BindAxis("CameraRight", this, &AGamer::PushCameraRight);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AGamer::ZoomCamera);
	PlayerInputComponent->BindAxis("CameraTurn", this, &AGamer::TurnCamera);
	PlayerInputComponent->BindAction("CameraToggleZoom", IE_Pressed, this, &AGamer::EnableCameraZoom);
	PlayerInputComponent->BindAction("CameraToggleZoom", IE_Released, this, &AGamer::DisableCameraZoom);

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetWorld()->GetFirstPlayerController(), nullptr, EMouseLockMode::LockInFullscreen, false);
}

void AGamer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdatePlayerLabel();
}

void AGamer::PushCameraUp(float AxisValue)
{
	if (AxisValue == 0) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC->IsLookInputIgnored())
	{
		// get current arm pitch
		FRotator Rotation = CameraArmComp->GetRelativeRotation();
		//Rotation.Pitch += FMath::RoundToFloat(AxisValue * 1.f);
		Rotation.Pitch += AxisValue * 3.0f;
		Rotation.Pitch = FMath::Clamp(Rotation.Pitch, -89.f, -30.f);

		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, FString::Printf(TEXT("Rot: %s"), *Rotation.ToCompactString()), true);

		CameraArmComp->SetRelativeRotation(Rotation);
	}
	//AddControllerPitchInput(AxisValue);
}

void AGamer::PushCameraRight(float AxisValue)
{
	
}

void AGamer::TurnCamera(float AxisValue)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (!bZoomToggled && !PC->IsLookInputIgnored())
	{
		//AddControllerYawInput(AxisValue * CameraRotationStep);

		FRotator Rotation = CameraArmComp->GetRelativeRotation();
		Rotation.Yaw += AxisValue * CameraRotationStep;
		CameraArmComp->SetRelativeRotation(Rotation);
	}
}

void AGamer::EnableCameraZoom()
{
	bZoomToggled = true;
	GEngine->AddOnScreenDebugMessage(-1, 0.6f, FColor::Red, TEXT("Zoom on"), false);
}

void AGamer::DisableCameraZoom()
{
	bZoomToggled = false;
	GEngine->AddOnScreenDebugMessage(-1, 0.6f, FColor::Red, TEXT("Zoom off"), false);
}

void AGamer::ZoomCamera(float AxisValue)
{
	if (bZoomToggled)
	{
		SetCameraZoom(AxisValue);

		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, FString::Printf(TEXT("Zoom: %f"), CameraZoomCurrent), true);
	}
}

float AGamer::SetCameraZoom(float InputDelta)
{
	float NewZoom = CameraZoomTarget + InputDelta * 500.0f;
	NewZoom = FMath::Clamp(NewZoom, CameraZoomMin, CameraZoomMax);
	CameraZoomTarget = NewZoom;
	return NewZoom;
}

void AGamer::MenuKeyPressed()
{
	AGameHUD* GameHUD = GetController<APlayerController>()->GetHUD<AGameHUD>();
	if (!ensure(IsValid(GameHUD))) return;

	GameHUD->ShowInGameMenu();
}

void AGamer::UpdatePlayerLabel()
{
	AGamerState* GamerState = GetPlayerState<AGamerState>();
	if (GamerState && PlayerLabel)
	{
		PlayerLabel->SetText(FText::FromString(GamerState->GetPlayerName()));
	}
}

void AGamer::Server_ThrowDie_Implementation()
{
	//TODO: Check if this player is actually in turn

	FDieThrow Throw;
	Throw.PlayerIndex = GetPlayerState<AGamerState>()->GetPlayerIndex();
	Throw.Result = static_cast<uint8>(FMath::RandRange(1, 6));

	UE_LOG(LogLudo, Verbose, TEXT("Player index %d throws a %d!"), Throw.PlayerIndex, Throw.Result);

	ALudoGameModeBase* GameMode = GetWorld()->GetAuthGameMode< ALudoGameModeBase>();
	GameMode->AddPlayerThrow(Throw);
}

bool AGamer::Server_ThrowDie_Validate()
{
	return true;
}

void AGamer::OnDieThrow(FDieThrow Throw)
{
	TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>();

	// check if this Gamer is relevant for the throw
	const int8 PlayerIndex = GetPlayerState<AGamerState>()->GetPlayerIndex();
	const bool bIsRelevant = PlayerIndex == Throw.PlayerIndex;
	if (!bIsRelevant) return;
	
	// Get the result of die thrown
	const bool bCanMovePiece = GameState->PlayerHasPieceOnBoard(PlayerIndex);
	//UE_LOG(LogLudo, Verbose, TEXT("%s: PlayerIndex %d rolled %d"), *GetName(), Throw.PlayerIndex, Throw.Result);

	if (Throw.Result == 1 || Throw.Result == 6)
	{
		UE_LOG(LogLudo, Verbose, TEXT("PlayerIndex %d can move a piece from Yard"), Throw.PlayerIndex);

		//GetPiece
	}

	/*if (bCanMovePiece)
	{
		// move a piece with Throw.Result
	}*/

	if (HasAuthority())
	{
		if (TObjectPtr<ILudoControllerInterface> ControllerInterface = Cast<ILudoControllerInterface>(GetController()))
		{
			ControllerInterface->Server_RequestEndTurn();
		}
	}
}