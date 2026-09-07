#pragma once

#include "ItemData/ItemsData.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS()
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	virtual void BeginPlay() override;

	bool AddToInventory(FInventoryItemSlot ItemToAdd);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	int32 GetInventoryCapacity() const;
	TArray<FInventoryItemSlot> GetActiveInventory() const;
	TArray<FInventoryItemSlot>& GetActiveInventoryRef();

	UFUNCTION(Server, Reliable)
	void Server_SwapItems(int32 SourceIndex, int32 TargetIndex);

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FStartingItemDef> StartingItems;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveInventory)
	TArray<FInventoryItemSlot> ActiveInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventoryCapacity = 0;

	UFUNCTION()
	void OnRep_ActiveInventory();

private:

};