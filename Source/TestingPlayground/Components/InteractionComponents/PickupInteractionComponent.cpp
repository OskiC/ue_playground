#include "PickupInteractionComponent.h"
#include <TestingPlayground/Components/InventoryComponents/InventoryComponent.h>

UPickupInteractionComponent::UPickupInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPickupInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ItemDefined.IsStructValid())
	{
		ItemPayload.ItemDefinition = ItemDefined.ItemDefinition;
		ItemPayload.StackCount = ItemDefined.Quantity;
	}
}

void UPickupInteractionComponent::OnInteract(APawn* Instigator)
{
	if (GetOwner()->HasAuthority())
	{
		if (ItemPayload.IsStructValid())
		{
			if (UInventoryComponent* PlayerInventory = Instigator->FindComponentByClass<UInventoryComponent>())
			{
				bool bSuccess = PlayerInventory->AddToInventory(ItemPayload);
				if (bSuccess)
				{
					UE_LOG(LogTemp, Warning, TEXT("destroyed parent: %s"), *GetOwner()->GetName());
					GetOwner()->Destroy();
				}
			}
		}

	}
}