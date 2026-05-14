#pragma once

#include "ItemData/ItemsData.h"

#include "InventoryComponent.generated.h"

UCLASS()
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	bool AddToInventory(FInventoryItemSlot ItemToAdd);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FStartingItemDef> StartingItems;

	UPROPERTY(Replicated)
	TArray<FInventoryItemSlot> ActiveInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventoryCapacity = 0;

private:

};