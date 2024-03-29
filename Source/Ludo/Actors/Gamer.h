// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>

#include "Game/LudoGameState.h"
#include "Gamer.generated.h"


class APlayerSlot;
class ABoard;
class AYard;
class APiece;

UCLASS(Abstract)
class LUDO_API AGamer : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* PlayerLabel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* ThrowWidget;

	bool bZoomToggled;
	float CameraZoomCurrent;
	float CameraZoomTarget;
	float CameraZoomDefault = 7000.f;
	float CameraZoomMin = 3000.f;
	float CameraZoomMax = 10000.f;
	float CameraRotationStep;

	UPROPERTY(Replicated, BlueprintGetter=GetPieces)
	TArray<APiece*> Pieces;

	UPROPERTY(Replicated)
	TObjectPtr<APlayerSlot> PlayerSlot;

	UPROPERTY(Replicated)
	TObjectPtr<AYard> PlayerYard;

	TObjectPtr<ALudoGameState> GameState;

public:
	// Sets default values for this pawn's properties
	AGamer();
	
	FORCEINLINE class USpringArmComponent* GetCameraArm() const { return CameraArmComp; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return CameraComp; }
	FORCEINLINE class UTextRenderComponent* GetPlayerLabel() const { return PlayerLabel; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Camera controls
	void PushCameraUp(float AxisValue);
	void PushCameraRight(float AxisValue);
	void TurnCamera(float AxisValue);
	void EnableCameraZoom();
	void DisableCameraZoom();
	void ZoomCamera(float AxisValue);

	float SetCameraZoom(float InputDelta);

	UFUNCTION()
	void MenuKeyPressed();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;

	virtual void Tick(float DeltaTime) override;
	
	void UpdatePlayerLabel();

	void AddPiece(TObjectPtr<APiece> Piece);

	UFUNCTION(BlueprintCallable)
	APiece* GetPiece(const uint8 AtIndex) const;

	UFUNCTION(BlueprintPure)
	TArray<APiece*> GetPieces() const { return Pieces; };

	UFUNCTION(BlueprintPure)
	TArray<APiece*> GetPiecesOnBoard(const ABoard* TheBoard, const bool bIncludeYard = false) const;

	UFUNCTION(BlueprintCallable)
	APlayerSlot* GetPlayerSlot() const { return PlayerSlot.Get(); };
	void SetPlayerSlot(TObjectPtr<APlayerSlot> Slot) { PlayerSlot = Slot; };

	UFUNCTION(BlueprintCallable)
	AYard* GetYard() const { return PlayerYard.Get(); };
	void SetYard(TObjectPtr<AYard> NewYard) { PlayerYard = NewYard; };

	//~=============================================================================
	// Player gameplay actions

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ThrowDie();

	void OnDieThrow(FDieThrow Throw);

	void EndAction() const;

	UFUNCTION(NetMulticast, Reliable)
	void Client_ShowEndScreen(APlayerSlot* WinnerSlot) const;
	
	//~=============================================================================
	// Visual

	UFUNCTION(BlueprintNativeEvent)
	void RunThrowUI(FDieThrow Throw) const;
};