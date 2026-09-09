#include "InventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/VerticalBox.h"

#include <TestingPlayground/PlayerCharacter/PlayerCharacter.h>
#include <TestingPlayground/PlayerState/CustomPlayerState.h>
#include <TestingPlayground/Components/InventoryComponents/EquipmentComponent.h>
#include <TestingPlayground/Components/InventoryComponents/InventoryComponent.h>
#include "TestingPlayground/Abilities/CustomAbilitySystemComponent.h"
#include "InventorySlotWidget.h"
#include <TestingPlayground/Widgets/MenuWidgets/DataAssets/AttributeUIData.h>
#include <TestingPlayground/Widgets/MenuWidgets/AttributeUIWidget.h>

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

	if (ACustomPlayerState* PlayerState = Cast<ACustomPlayerState>(GetOwningPlayerState()))
	{
		if (UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent())
		{
			if (IsValid(AttributeUIDataAsset))
			{
				for (const FAttributeUIData& AttributeSlot : AttributeUIDataAsset->AttributesToShow)
				{
					ASC->GetGameplayAttributeValueChangeDelegate(AttributeSlot.Attribute).AddUObject(this, &UInventoryWidget::OnAttributeValueChanged);
				}
			}
		}
	}

	PopulateInventoryGrid();
	PopulateEquipmentGrid();
	PopulateAttributeBox();
}

void UInventoryWidget::OnAttributeValueChanged(const FOnAttributeChangeData& Data)
{
	PopulateAttributeBox();
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
			NewSlot->OnItemDropped.AddDynamic(this, &UInventoryWidget::HandleSlotDrop);

			int32 Row = i / Columns;
			int32 Col = i % Columns;

			UUniformGridSlot* GridSlot = InventoryGridPanel->AddChildToUniformGrid(NewSlot, Row, Col);
			if (GridSlot)
			{
				GridSlot->SetHorizontalAlignment(HAlign_Center);
				GridSlot->SetVerticalAlignment(VAlign_Top);
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
			//NewSlot->SetPadding({ 50.f, 50.f });

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
			NewSlot->OnItemDropped.AddDynamic(this, &UInventoryWidget::HandleSlotDrop);

			if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(EquipmentPanel->AddChild(NewSlot)))
			{
				GridSlot->SetRow(EquipmentSlots[i].GridRow);
				GridSlot->SetColumn(EquipmentSlots[i].GridColumn);

				GridSlot->SetHorizontalAlignment(HAlign_Center);
				GridSlot->SetVerticalAlignment(VAlign_Center);
			}
		}
	}
}

void UInventoryWidget::HandleSlotDrop(EPanelType SourcePanel, int32 SourceIndex, EPanelType TargetPanel, int32 TargetIndex)
{
	// Swap functions calls delegate to populate equipment and inventory grids
	if (SourcePanel == EPanelType::Inventory && TargetPanel == EPanelType::Inventory)
	{
		InventoryComponent->Server_SwapItems(SourceIndex, TargetIndex);
	}
	else if (SourcePanel == EPanelType::Inventory && TargetPanel == EPanelType::Equipment)
	{
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
		{
			Character->Server_EquipItemFromInventory(SourceIndex, TargetIndex);
		}
	}
	else if (SourcePanel == EPanelType::Equipment && TargetPanel == EPanelType::Inventory)
	{
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
		{
			Character->Server_UnequipItemToInventory(SourceIndex, TargetIndex);
		}
	}
	else if (SourcePanel == EPanelType::Equipment && TargetPanel == EPanelType::Equipment)
	{
		UE_LOG(LogTemp, Warning, TEXT("FROM EQ TO EQ probably not needed but we'll see"));
	}
}

void UInventoryWidget::PopulateAttributeBox()
{
	if (!IsValid(AttributeWidgetClass))
	{
		ensureAlwaysMsgf(false, TEXT("Setup AttributeWidgetClass in Defaults in InventoryWidget"));
		return;
	}
	if (!IsValid(AttributeUIDataAsset))
	{
		ensureAlwaysMsgf(false, TEXT("Setup AttributeWidgetDataAsset in InventoryWidget"));
		return;
	}

	AttributeBox->ClearChildren();

	for (FAttributeUIData& AttributeSlot : AttributeUIDataAsset->AttributesToShow)
	{
		UAttributeUIWidget* NewSlot = CreateWidget<UAttributeUIWidget>(this, AttributeWidgetClass);
		
		if (IsValid(NewSlot))
		{
			UTexture2D* AttributeIcon = AttributeSlot.AttributeIcon;
			FText AttributeName = AttributeSlot.AttributeName;
			float AttributeVal = 0.f;

			if (ACustomPlayerState* PlayerState = Cast<ACustomPlayerState>(GetOwningPlayerState()))
			{
				if (UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent())
				{
					AttributeVal = ASC->GetNumericAttribute(AttributeSlot.Attribute);
				}
			}

			if (IsValid(AttributeIcon) && !AttributeName.IsEmpty())
			{
				NewSlot->SetupAttributeSlot(AttributeIcon, AttributeName, AttributeVal);
			}

			AttributeBox->AddChildToVerticalBox(NewSlot);
		}
	}
}