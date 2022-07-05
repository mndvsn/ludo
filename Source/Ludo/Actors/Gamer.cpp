// Copyright Martin Furuberg. All Rights Reserved.


#include "Gamer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Game/LudoGameInstance.h"
#include "Game/GamerState.h"
#include "UI/DefaultHUD.h"
#include "LudoPlayerController.h"

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
	
	// Don't auto possess (handled by game mode)
	//AutoPossessPlayer = EAutoReceiveInput::Disabled;

	// Find UI class
	ConstructorHelpers::FClassFinder<UUserWidget> DefaultHUD_BPClass(TEXT("/Game/UI/W_Game_QuickMenu"));
	if (DefaultHUD_BPClass.Class) {
		DefaultHUD_Class = DefaultHUD_BPClass.Class;
	}
}

// Called when the game starts or when spawned
void AGamer::BeginPlay()
{
	Super::BeginPlay();

	//APlayerController* PC = GetWorld()->GetFirstPlayerController();
	CameraRotationStep = (360.0f / 4);// / PC->InputYawScale; // 90

	//PC->SetControlRotation(FRotator(-80.f, 0, 0));

	// Put PlayerIndex in PlayerLabel
	//UpdatePlayerLabel();
	
	// Create and display HUD
	if (IsLocallyControlled() && DefaultHUD_Class != nullptr)
	{
		DefaultHUD = Cast<UDefaultHUD>(CreateWidget(GetWorld(), DefaultHUD_Class));
		DefaultHUD->AddToViewport();
		DefaultHUD->OnShowMenu.AddDynamic(this, &AGamer::ShowInGameMenu);
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
	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &AGamer::ShowInGameMenu);

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
	
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Blue, FString::Printf(TEXT("OnRep: %s"), *GetPlayerState()->GetPlayerName()), true);
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

void AGamer::UpdatePlayerLabel()
{
	AGamerState* GamerState = GetPlayerState<AGamerState>();
	if (GamerState && PlayerLabel)
	{
		PlayerLabel->SetText(FText::FromString(GamerState->GetPlayerName()));
	}
}

void AGamer::ShowInGameMenu()
{
	ULudoGameInstance* GameInstance = GetGameInstance<ULudoGameInstance>();
	if (GameInstance == nullptr) return;
	GameInstance->EndGame();
}