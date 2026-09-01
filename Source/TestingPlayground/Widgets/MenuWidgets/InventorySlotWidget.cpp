#include "InventorySlotWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Input/Reply.h"

void UInventorySlotWidget::SetupSlot(int32 SlotIndex, EPanelType PanelType, const FInventoryItemSlot& SlotData, UTexture2D* GhostIcon)
{
	MyInventoryIndex = SlotIndex;
	MyPanelType = PanelType;

	if (!ItemIcon || !CountText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item not created"));
		return;
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
		else if(SlotData.StackCount == 1)
		{
			CountText->SetText(FText::AsNumber(SlotData.StackCount));
			CountText->SetVisibility(ESlateVisibility::Hidden);
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

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!IsValid(this) || this->CountText->GetText().IsEmpty())
	{
		return;
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UItemDragDropOperation* Payload = NewObject<UItemDragDropOperation>();
	Payload->SourceIndex = MyInventoryIndex;
	Payload->SourceWidget = this;
	Payload->PanelSource = MyPanelType;
	Payload->DefaultDragVisual = this;
	
	OutOperation = Payload;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (UItemDragDropOperation* Payload = Cast<UItemDragDropOperation>(InOperation))
	{
		if (Payload->SourceWidget != this)
		{
			OnItemDropped.Broadcast(Payload->PanelSource, Payload->SourceIndex, MyPanelType, MyInventoryIndex);
			return true;
		}
	}

	return false;
}