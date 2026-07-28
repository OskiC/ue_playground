#include "InventoryWidget.h"

#include <TestingPlayground/PlayerCharacter/PlayerCharacter.h>
#include <TestingPlayground/Components/InventoryComponents/EquipmentComponent.h>
#include <TestingPlayground/Components/InventoryComponents/InventoryComponent.h>

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "InventorySlotWidget.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FetchCharacterInfromation();
}

void UInventoryWidget::FetchCharacterInfromation()
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (!IsValid(Character))
	{
		return;
	}

	InventoryComponent = Character->FindComponentByClass<UInventoryComponent>();
	EquipmentComponent = Character->FindComponentByClass<UEquipmentComponent>();
	if (!IsValid(InventoryComponent) || !IsValid(EquipmentComponent))
	{
		return;
	}

	InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::PopulateInventoryGrid);
	EquipmentComponent->OnEquipmentUpdated.AddDynamic(this, &UInventoryWidget::PopulateEquipmentGrid);

	PopulateInventoryGrid();
	PopulateEquipmentGrid();
}

void UInventoryWidget::PopulateInventoryGrid()
{
	if (!IsValid(InventoryGridPanel))
	{
		return;
	}
	else
	{
		InventoryGridPanel->ClearChildren();
	}

	if (!InventorySlotWidgetClass)
	{
		ensureAlwaysMsgf(false, TEXT("Ensure that inventory slot widget class is set up"));
		return;
	}

	int32 InventoryCapacity = InventoryComponent->GetInventoryCapacity();
	const TArray<FInventoryItemSlot> ItemsInInventory = InventoryComponent->GetActiveInventory();
	
	for (int32 i = 0; i < InventoryCapacity; i++)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

		if (IsValid(NewSlot))
		{
			FInventoryItemSlot SlotDataToPass;
			if (ItemsInInventory.IsValidIndex(i))
			{
				SlotDataToPass = ItemsInInventory[i];
			}

			NewSlot->SetupSlot(i, EPanelType::Inventory, ItemsInInventory[i]);

			int32 Row = i / Columns;
			int32 Col = i % Columns;

			UUniformGridSlot* GridSlot = InventoryGridPanel->AddChildToUniformGrid(NewSlot, Row, Col);
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(HAlign_Center);
				GridSlot->SetVerticalAlignment(VAlign_Center);
			}
		}
	}
}

void UInventoryWidget::PopulateEquipmentGrid()
{
	if (!IsValid(EquipmentPanel) || !IsValid(EquipmentComponent))
	{
		return;
	}
	EquipmentPanel->ClearChildren();

	int32 EquipmentSlotCount = EquipmentComponent->GetItemSlotCount();

	const TArray<FEquipItemSlot>& EquipmentSlots = EquipmentComponent->GetEquipmentSlots();

	for (int32 i = 0; i < EquipmentSlotCount; i++)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

		if (IsValid(NewSlot))
		{
			FInventoryItemSlot PayloadForUI;
			UTexture2D* GhostIconToPass = nullptr;
			NewSlot->SetPadding({ 50.f, 50.f });

			if (EquipmentSlots.IsValidIndex(i))
			{
				GhostIconToPass = EquipmentSlots[i].EmptySlotIcon;

				if (IsValid(EquipmentSlots[i].EquippedItem))
				{
					PayloadForUI.ItemInstance = EquipmentSlots[i].EquippedItem;
					PayloadForUI.StackCount = 1;
				}
			}

			NewSlot->SetupSlot(i, EPanelType::Equipment, PayloadForUI, GhostIconToPass);

			EquipmentPanel->AddChild(NewSlot);
		}
	}
}