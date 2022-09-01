// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Common/PlayerCore.h"
#include "Piece.generated.h"


UCLASS(Abstract)
class LUDO_API APiece : public AActor
{
	GENERATED_BODY()

public:
	APiece();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> ColorMaterialInstance;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece", ReplicatedUsing=OnRep_PlayerCore)
	FPlayerCore PlayerCore;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_PlayerCore();
	
	UPROPERTY(Replicated)
	bool bInYard;

	UPROPERTY(Replicated)
	bool bInGoal;
	
	FVector InitialLocation;

	virtual void BeginPlay() override;

public:
	FPlayerCore GetPlayerCore() const { return PlayerCore; }
	void SetPlayerCore(FPlayerCore InPlayerCore);

	bool IsInYard() const { return bInYard; };
	void SetInYard(bool bIn) { bInYard = bIn; };

	bool IsInGoal() const { return bInGoal; };
	void SetInGoal(bool bIn) { bInGoal = bIn; };

	FVector GetInitialLocation() const { return InitialLocation; }
	void SetInitialLocation(const FVector& InLocation) { InitialLocation = InLocation; }
};