#pragma once

#include <GameplayTagContainer.h>

#include "ItemsData.generated.h"

UCLASS()
class UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString ItemName = TEXT("");

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D>  ItemIcon = nullptr;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer ItemTags;
};

USTRUCT(BlueprintType)
struct FStartingItemDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemDefinition> ItemDefinition = nullptr;

	UPROPERTY(EditAnywhere, meta=(ClampMin="1"))
	int32 Quantity = 1;

	bool IsStructValid() const
	{
		if (IsValid(ItemDefinition))
		{
			return true;
		}

		return false;
	}
};

USTRUCT(BlueprintType)
struct FInventoryItemSlot
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UItemDefinition> ItemDefinition = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	bool IsStructValid() const
	{
		if (IsValid(ItemDefinition))
		{
			return true;
		}

		return false;
	}
};