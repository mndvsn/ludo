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
	uint8 Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(TitleProperty=DisplayName))
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor PrimaryColor;

	FPlayerCore()
	{
		Id = 255;
		DisplayName = "Color";
		PrimaryColor = FColor::FromHex("FF00FF");
	};

	inline bool operator==(const FPlayerCore& rhs) const
	{
		return Id == rhs.Id;
	}
};

/**
 * 
 */
UCLASS()
class LUDO_API UPlayerCoreAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlayerCore> Cores;
};
