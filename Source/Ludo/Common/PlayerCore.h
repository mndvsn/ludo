// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerCore.generated.h"

/**
 * 
 */
UCLASS()
class LUDO_API UPlayerCore : public UDataAsset
{
	GENERATED_BODY()

public:
	UPlayerCore();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(TitleProperty=DisplayName))
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor PrimaryColor;

};
