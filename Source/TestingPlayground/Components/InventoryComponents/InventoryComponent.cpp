#include "InventoryComponent.h"
#include <Net/UnrealNetwork.h>

UInventoryComponent::UInventoryComponent()
{

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, ActiveInventory);
}

bool UInventoryComponent::AddToInventory(FInventoryItemSlot ItemToAdd)
{
	UItemDefinition* ItemType = ItemToAdd.ItemDefinition;
	for (auto& Item : ActiveInventory)
	{
		if (Item.ItemDefinition == ItemType)
		{
			Item.StackCount += ItemToAdd.StackCount;
			return true;
		}
	}

	if (ActiveInventory.Num() >= InventoryCapacity)
	{
		return false;
	}

	ActiveInventory.Emplace(ItemToAdd);
	return true;
}