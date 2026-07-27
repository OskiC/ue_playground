#include "InventorySlotWidget.h"

#include "../../Components/InventoryComponents/ItemData/ItemsData.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInventorySlotWidget::SetupSlot(int32 SlotIndex, const FInventoryItemSlot& SlotData, UTexture2D* GhostIcon)
{
	MyInventoryIndex = SlotIndex;

	if (!ItemIcon || !CountText || !SlotButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item not created"));
		return;
	}
	
	if (!SlotButton->OnClicked.IsAlreadyBound(this, &UInventorySlotWidget::OnSlotClicked))
	{
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClicked);
	}

	if (SlotData.IsStructValid())
	{
		UItemDefinition* ItemDef = SlotData.ItemInstance->ItemDef;

		if (IsValid(ItemDef->ItemIcon))
		{
			ItemIcon->SetBrushFromTexture(ItemDef->ItemIcon);
			ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (SlotData.StackCount > 1)
		{
			CountText->SetText(FText::AsNumber(SlotData.StackCount));
			CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CountText->SetText(FText::GetEmpty());
		}
	}
	else
	{
		// Slot is empty
		CountText->SetText(FText::GetEmpty());
		
		if (IsValid(GhostIcon))
		{
			ItemIcon->SetBrushFromTexture(GhostIcon);
			ItemIcon->SetRenderOpacity(0.3f);
			ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventorySlotWidget::OnSlotClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Button clicked on item"));
}