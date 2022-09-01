// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/PlayerCore.h"
#include "Yard.generated.h"


class APiece;
class AGamer;
class APlayerSquare;

UCLASS(Abstract)
class LUDO_API AYard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYard();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> ColorMaterialInstance;

	void SpawnPieces();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_PlayerCore, Category="Yard")
	FPlayerCore PlayerCore;

	UPROPERTY(EditDefaultsOnly, Category="Yard")
	TSoftClassPtr<APiece> PieceClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_PlayerCore();

private:
	UPROPERTY(Replicated, BlueprintGetter=GetGamer)
	TObjectPtr<AGamer> Gamer;

public:	
	UPROPERTY(EditAnywhere)
	int32 Index = -1;

	UFUNCTION(BlueprintPure)
	AGamer* GetGamer() const { return Gamer; };

	UFUNCTION(BlueprintPure)
	APlayerSquare* GetHomeSquare() const { return PlayerSquares[0]; };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<APlayerSquare*> PlayerSquares;

	void SetGamer(TObjectPtr<AGamer> NewGamer);
};
