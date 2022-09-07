// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Common/PlayerCore.h"
#include "Piece.generated.h"


class AGamer;
class APiece;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAE_OnAnimatePathFinished);
DECLARE_DELEGATE(FAE_OnAnimatePathFinishedNative);

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

	UPROPERTY(BlueprintCallable, Category=Events)
	FAE_OnAnimatePathFinished OnAnimatePathFinished;

private:
	FAE_OnAnimatePathFinishedNative OnAnimatePathFinishedNative;

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

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> MovesArray;
	
	virtual void BeginPlay() override;
	
public:
	const FPlayerCore& GetPlayerCore() const { return PlayerCore; }
	void SetPlayerCore(const FPlayerCore& InPlayerCore);

	bool IsInYard() const { return bInYard; };
	void SetInYard(bool bIn) { bInYard = bIn; };

	bool IsInGoal() const { return bInGoal; };
	void SetInGoal(bool bIn) { bInGoal = bIn; };

	UFUNCTION(BlueprintPure)
	AGamer* GetGamer() const;
	
	FVector GetInitialLocation() const { return InitialLocation; }
	void SetInitialLocation(const FVector& InLocation) { InitialLocation = InLocation; }

	UFUNCTION(BlueprintNativeEvent)
	void AnimatePath(const TArray<FVector>& Path, const bool bHandleFinished = false);

	UFUNCTION()
	void HandleAnimatePathFinished();

	virtual FAE_OnAnimatePathFinishedNative& GetAnimatePathFinishedDelegate() { return OnAnimatePathFinishedNative; }
};