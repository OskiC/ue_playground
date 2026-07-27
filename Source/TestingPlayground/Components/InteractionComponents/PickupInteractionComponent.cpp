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
		TooltipText = FText::FromString(ItemDefined.ItemDefinition->ItemName);

		if (GetOwner()->HasAuthority())
		{
			UItemInstance* CreatedInstance = NewObject<UItemInstance>(this);
			CreatedInstance->ItemDef = ItemDefined.ItemDefinition;

			ItemPayload.ItemInstance = CreatedInstance;
			ItemPayload.StackCount = ItemDefined.Quantity;
		}
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
				UItemInstance* SafeInstance = DuplicateObject<UItemInstance>(ItemPayload.ItemInstance, PlayerInventory);
				FInventoryItemSlot SafePayload = ItemPayload;
				SafePayload.ItemInstance = SafeInstance;

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