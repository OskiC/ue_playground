#include "InventoryComponent.h"

#include <Net/UnrealNetwork.h>
#include "Engine/ActorChannel.h"

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ActiveInventory.Empty();
	ActiveInventory.SetNum(InventoryCapacity);
	// in the future add some starting items
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, ActiveInventory);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (const FInventoryItemSlot& Slot : ActiveInventory)
	{
		if (Slot.IsStructValid())
		{
			bWroteSomething |= Channel->ReplicateSubobject(Slot.ItemInstance, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

bool UInventoryComponent::AddToInventory(FInventoryItemSlot ItemToAdd)
{
	if (!GetOwner()->HasAuthority())
	{
		return false;
	}

	if (!ItemToAdd.IsStructValid() || ItemToAdd.StackCount <= 0)
	{
		return false;
	}

	int32 AmountRemaining = ItemToAdd.StackCount;
	int32 MaxStack = ItemToAdd.ItemInstance->ItemDef->MaxStackSize;
	bool bInventoryChanged = false;

	for (FInventoryItemSlot& ExistingSlot : ActiveInventory)
	{
		if (ExistingSlot.IsStructValid() && ExistingSlot.ItemInstance->ItemDef == ItemToAdd.ItemInstance->ItemDef)
		{
			int32 RoomLeftInStack = MaxStack - ExistingSlot.StackCount;
			if (RoomLeftInStack > 0)
			{
				int32 AmountToTake = FMath::Min(AmountRemaining, RoomLeftInStack);
				ExistingSlot.StackCount += AmountToTake;
				AmountRemaining -= AmountToTake;
				bInventoryChanged = true;

				if (AmountRemaining <= 0)
				{
					break;
				}
			}
		}
	}

	while (AmountRemaining > 0)
	{
		int32 EmptyIndex = INDEX_NONE;

		for (int32 i = 0; i < ActiveInventory.Num(); i++)
		{
			if (!ActiveInventory[i].IsStructValid())
			{
				EmptyIndex = i;
				break;
			}
		}

		if (EmptyIndex == INDEX_NONE)
		{
			break;
		}

		int32 AmountForNewSlot = FMath::Min(AmountRemaining, MaxStack);

		ActiveInventory[EmptyIndex].ItemInstance = ItemToAdd.ItemInstance;
		ActiveInventory[EmptyIndex].StackCount = AmountForNewSlot;

		AmountRemaining -= AmountForNewSlot;
		bInventoryChanged = true;
	}

	if (bInventoryChanged)
	{
		OnInventoryUpdated.Broadcast();
	}

	return AmountRemaining <= 0;
}

TArray<FInventoryItemSlot> UInventoryComponent::GetActiveInventory() const
{
	return ActiveInventory;
}

int32 UInventoryComponent::GetInventoryCapacity() const
{
	return InventoryCapacity;
}

void UInventoryComponent::OnRep_ActiveInventory()
{
	OnInventoryUpdated.Broadcast();
}