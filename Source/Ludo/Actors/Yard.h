// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Yard.generated.h"


class UPlayerCore;
class APiece;
class AGamer;

UCLASS()
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yard")
	TObjectPtr<UPlayerCore> PlayerCore;

	UPROPERTY(EditDefaultsOnly, Category="Yard")
	TSoftClassPtr<APiece> PieceClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintGetter=GetGamer)
	TObjectPtr<AGamer> Gamer;

public:	
	UPROPERTY(EditAnywhere)
	int32 Index = -1;

	UFUNCTION(BlueprintPure)
	AGamer* GetGamer() { return Gamer; };

	void SetGamer(TObjectPtr<AGamer> NewGamer) { Gamer = NewGamer; };
};
