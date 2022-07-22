// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Square.generated.h"


UCLASS()
class LUDO_API ASquare : public AActor
{
	GENERATED_BODY()
	
public:	
	ASquare();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	
	void AddNext(TObjectPtr<ASquare> NewSquare);
	TArray<TObjectPtr<ASquare>> GetNext() { return Next; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ASquare>> Next;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetHighlight(bool Highlighted);

	FString GetDebugText();

	UPROPERTY(EditAnywhere)
	int32 Index = -1;
};
