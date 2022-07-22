// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Piece.generated.h"


class UPlayerCore;

UCLASS()
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
	TObjectPtr<UPlayerCore> PlayerCore;

	UFUNCTION()
	void OnRep_PlayerCore();

	virtual void BeginPlay() override;

public:
	void SetPlayerCore(TObjectPtr<UPlayerCore> InPlayerCore);

};