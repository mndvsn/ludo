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
	
	void AddNext(ASquare* NewSquare);
	TArray<ASquare*> GetNext() { return Next; };

protected:
	virtual void BeginPlay() override;

	TArray<ASquare*> Next;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetHighlight(bool Highlighted);

	FString GetDebugText();
	int32 Index = -1;
};
