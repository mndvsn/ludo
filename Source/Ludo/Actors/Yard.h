// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Yard.generated.h"


class UPlayerCore;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yard")
	TObjectPtr<UPlayerCore> PlayerCore;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


};
