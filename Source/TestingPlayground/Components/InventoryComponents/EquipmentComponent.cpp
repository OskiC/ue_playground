#include "EquipmentComponent.h"

#include <Net/UnrealNetwork.h>
#include "Engine/ActorChannel.h"

UEquipmentComponent::UEquipmentComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, EquippedItems);
}

bool UEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (const FEquipItemSlot& Slot : EquippedItems)
	{
		if (Slot.IsStructValid())
		{
			bWroteSomething |= Channel->ReplicateSubobject(Slot.EquippedItem, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

int32 UEquipmentComponent::GetItemSlotCount()
{
	return EquippedItems.Num();
}

TArray<FEquipItemSlot> UEquipmentComponent::GetEquipmentSlots() const
{
	return EquippedItems;
}

bool UEquipmentComponent::EquipItem(UItemInstance* ItemToEquip)
{
	if (GetOwner()->HasAuthority() || !IsValid(ItemToEquip))
	{
		return false;
	}

	bool bEquipped = false;

	for (auto& ItemSlot : EquippedItems)
	{
		if (ItemToEquip->ItemDef->ItemTags.HasTag(ItemSlot.SlotRequirementTag))
		{
			// we can add here more validations like level etc. in future
			ItemSlot.EquippedItem = ItemToEquip;
			bEquipped = true;
			break;
		}
	}

	if (bEquipped)
	{
		OnEquipmentUpdated.Broadcast();
	}

	return bEquipped;
}

void UEquipmentComponent::OnRep_EquippedItems()
{
	OnEquipmentUpdated.Broadcast();
}