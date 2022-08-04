// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Gamer.generated.h"


struct FDieThrow;
class APiece;
class APlayerSlot;
class AYard;

UCLASS()
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

	bool bZoomToggled;
	float CameraZoomCurrent;
	float CameraZoomTarget;
	float CameraZoomDefault = 7000.f;
	float CameraZoomMin = 3000.f;
	float CameraZoomMax = 10000.f;
	float CameraRotationStep;

	UPROPERTY(BlueprintGetter=GetPieces)
	TArray<TObjectPtr<APiece>> Pieces;

	UPROPERTY()
	TObjectPtr<APlayerSlot> PlayerSlot;

	UPROPERTY()
	TObjectPtr<AYard> PlayerYard;

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
	APiece* GetPiece(uint8 AtIndex);

	UFUNCTION(BlueprintPure)
	TArray<APiece*> GetPieces() { return Pieces; };

	UFUNCTION(BlueprintCallable)
	APlayerSlot* GetPlayerSlot() { return PlayerSlot.Get(); };
	void SetPlayerSlot(TObjectPtr<APlayerSlot> Slot) { PlayerSlot = Slot; };

	UFUNCTION(BlueprintCallable)
	AYard* GetYard() { return PlayerYard.Get(); };
	void SetYard(TObjectPtr<AYard> NewYard) { PlayerYard = NewYard; };

	//~=============================================================================
	// Player gameplay actions

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ThrowDie();

	void OnDieThrow(FDieThrow Throw);
};