// Copyright Martin Furuberg. All Rights Reserved.


#include "Gamer.h"

#include <Net/UnrealNetwork.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/TextRenderComponent.h>
#include <Components/WidgetComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

#include "LudoLog.h"
#include "Game/LudoGameModeBase.h"
#include "Game/LudoGameState.h"
#include "Game/LudoPlayerController.h"
#include "Game/GamerState.h"
#include "Actors/Board.h"
#include "Actors/Piece.h"
#include "Actors/PlayerSquare.h"
#include "Actors/Yard.h"
#include "Actors/PlayerSlot.h"
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

	ThrowWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ThrowWidget"));
	ThrowWidget->SetupAttachment(PlayerLabel);
}

void AGamer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamer, PlayerSlot);
	DOREPLIFETIME(AGamer, PlayerYard);
	DOREPLIFETIME(AGamer, Pieces);
}

// Called when the game starts or when spawned
void AGamer::BeginPlay()
{
	Super::BeginPlay();

	CameraRotationStep = (360.0f / 4);// / PC->InputYawScale; // 90

	if (const TObjectPtr<ALudoGameState> GameState = GetWorld()->GetGameState<ALudoGameState>())
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

		const FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraComp->GetComponentLocation(), FVector::ZeroVector);
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

	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
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
	//TODO: PushCamerRight
}

void AGamer::TurnCamera(float AxisValue)
{
	const APlayerController* PC = GetWorld()->GetFirstPlayerController();

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
	const AGamerState* GamerState = GetPlayerState<AGamerState>();
	if (GamerState && PlayerLabel)
	{
		PlayerLabel->SetText(FText::FromString(GamerState->GetPlayerName()));
	}
}

void AGamer::AddPiece(TObjectPtr<APiece> Piece)
{
	if (!Piece || !HasAuthority()) return;

	Pieces.Add(Piece);
}

APiece* AGamer::GetPiece(const uint8 AtIndex) const
{
	if (!Pieces.IsValidIndex(AtIndex)) return nullptr;

	return Pieces[AtIndex];
}

TArray<APiece*> AGamer::GetPiecesOnBoard(const ABoard* TheBoard) const
{
	TArray<APiece*> PiecesOnBoard = Pieces.FilterByPredicate([&](const APiece* Piece)
	{
		return !Piece->IsInGoal() && !Piece->IsInYard();
	});

	return PiecesOnBoard;
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

void AGamer::OnDieThrow(FDieThrow Throw) const
{
	// check if this Gamer is relevant for the throw
	const int8 PlayerIndex = GetPlayerState<AGamerState>()->GetPlayerIndex();
	const bool bIsRelevant = PlayerIndex == Throw.PlayerIndex;
	if (!bIsRelevant) return;

	// Display number
	RunThrowUI(Throw);
	
	// Stop client here, for now
	if (!HasAuthority()) return;

	const ALudoPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ALudoPlayerController>();
	if (!PlayerController) return;

	const TObjectPtr<ABoard> TheBoard = PlayerController->TheBoard;
	
	// Check if player has piece on board
	bool bCanMovePiece = GetPiecesOnBoard(TheBoard).Num() > 0;
	bool bMadeMove = false;
	TObjectPtr<APiece> PieceMoveSpecific = nullptr;

	// Get game rules
	const TObjectPtr<ALudoGameModeBase> GameMode = GetWorld()->GetAuthGameMode<ALudoGameModeBase>();
	
	// Get the result of dice roll
	if (GameMode->GetEntryRolls(EEntryRollMask::ER_All).Contains(Throw.Result))
	{
		if (APiece* Piece = GetYard()->GetPiece())
		{
			UE_LOG(LogLudo, Verbose, TEXT("Player %d can move a piece from their yard"), Throw.PlayerIndex);

			// Try to move Piece to player home square
			APlayerSquare* HomeSquare = GetYard()->GetHomeSquare();
			TheBoard->MovePiece(Piece, HomeSquare);

			// Immediately move on a high roll?
			if (GameMode->bMoveOnHighEntryRoll && GameMode->GetEntryRolls(EEntryRollMask::ER_High).Contains(Throw.Result))
			{
				bCanMovePiece = true;
				PieceMoveSpecific = Piece;

				// Subtract movement to home
				Throw.Result -= 1;
			}
			else
			{
				bMadeMove = true;
			}
		}
	}

	// Move piece on board
	if (bCanMovePiece && !bMadeMove)
	{
		short Attempt = 0;
		
		TArray<APiece*> MovablePieces;
		if (PieceMoveSpecific)
		{
			MovablePieces.Add(PieceMoveSpecific);
		}
		else
		{
			MovablePieces = GetPiecesOnBoard(TheBoard);
		}

		while (!bMadeMove)
		{
			if (MovablePieces.IsValidIndex(Attempt))
			{
				APiece* Piece = MovablePieces[Attempt];
				const uint8 StartIndex = TheBoard->IndexOfSquare(TheBoard->LocationOfPiece(Piece));

				TArray<TObjectPtr<ASquare>> SquaresAhead = TheBoard->GetReachableSquares(StartIndex, Throw.Result, Throw.PlayerIndex);

				// If squares are fewer than throw it means we are moving a piece beyond goal, choose next piece
				if (SquaresAhead.Num() == Throw.Result)
				{
					TheBoard->MovePiece(Piece, SquaresAhead.Last());
					bMadeMove = true;
				}
				Attempt++;
			}
			else
			{
				// Ran out of moves
				bMadeMove = true;
			}
		}
	}

	if (const TObjectPtr<ILudoControllerInterface> ControllerInterface = Cast<ILudoControllerInterface>(GetController()))
	{
		ControllerInterface->Server_RequestEndTurn();
	}
}

void AGamer::RunThrowUI_Implementation(FDieThrow Throw) const
{
	// const TObjectPtr<ALudoPlayerController> LudoPC = GetWorld()->GetFirstPlayerController<ALudoPlayerController>();
	// if (!LudoPC) return;
	//
	// if (const TObjectPtr<AGameHUD> GameHUD = LudoPC->GetHUD<AGameHUD>())
	// {
	// 	GameHUD->ShowThrow();
	// }
}

void AGamer::Client_ShowEndScreen_Implementation(APlayerSlot* WinnerSlot) const
{
	const TObjectPtr<ALudoPlayerController> LudoPC = GetWorld()->GetFirstPlayerController<ALudoPlayerController>();
	if (!LudoPC || !WinnerSlot) return;
	
	if (const TObjectPtr<AGameHUD> GameHUD = LudoPC->GetHUD<AGameHUD>())
	{
		GameHUD->ShowEndScreen(WinnerSlot);
	}
}
