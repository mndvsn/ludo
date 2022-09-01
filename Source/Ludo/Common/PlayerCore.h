// Copyright Martin Furuberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerCore.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FPlayerCore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Id = 255;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(TitleProperty=DisplayName))
	FString DisplayName = "Color";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor PrimaryColor = FColor::Magenta;

	inline bool operator==(const FPlayerCore& rhs) const
	{
		return Id == rhs.Id;
	}
};

/**
 * 
 */
UCLASS(NotPlaceable)
class LUDO_API UPlayerCoreAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerCore> Cores;
};
