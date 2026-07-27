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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 MaxStackSize = 1;
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

UCLASS(BlueprintType)
class UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<UItemDefinition> ItemDef = nullptr;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FGameplayTagContainer InstanceTags;
};

USTRUCT(BlueprintType)
struct FInventoryItemSlot
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	bool IsStructValid() const
	{
		return IsValid(ItemInstance);
	}
};

USTRUCT(BlueprintType)
struct FEquipItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FGameplayTag SlotRequirementTag;

	UPROPERTY()
	TObjectPtr<UItemInstance> EquippedItem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TObjectPtr<UTexture2D> EmptySlotIcon = nullptr;

	bool IsStructValid() const
	{
		return IsValid(EquippedItem);
	}
};